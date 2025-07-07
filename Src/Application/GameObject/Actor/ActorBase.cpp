#include "ActorBase.h"

#include "../../Factory/Factory.h"

#include "../../Utility/JsonUtility.h"
#include "../../Utility/ImGui/ImGuiManager.h"

#include "../../main.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/BaseScene/BaseScene.h"

void ActorBase::Init()
{
	KdGameObject::Init();
	
	m_transform   = {};
	m_meshInfo    = {};
	m_gravityInfo = {};

	m_movement      = Math::Vector3::Zero;
	m_moveDirection = Math::Vector3::Zero;

	m_meshInfo.assetFilePath = COMMON_ASSET_FILE_PATH;
}

void ActorBase::PostLoadInit()
{
	LoadAsset();
}

void ActorBase::DrawLit()
{
	if (!m_mesh) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_mesh, m_mWorld , m_meshInfo.color);
}

void ActorBase::GenerateDepthMapFromLight()
{
	if (!m_mesh) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_mesh, m_mWorld, m_meshInfo.color);
}

void ActorBase::PostUpdate()
{
	FixMatrix   ();
	MapCollision();
}

void ActorBase::DrawImGuiInspectors()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");
	DrawImGuiTransformInspector();

	imGui_.DrawSeparate();
	ImGui::Text("Material");
	DrawImGuiMaterialInspector ();
}
void ActorBase::DrawImGuiTransformInspector()
{
	ImGui::DragFloat3("Location", &m_transform.location.x, 0.1f);
	ImGui::DragFloat3("Rotation", &m_transform.rotation.x, 1.0f);
	ImGui::DragFloat3("Scale", &m_transform.scale.x, 0.1f);
}
void ActorBase::DrawImGuiMaterialInspector()
{
	if (ImGui::Button(("TextureFilePath : %s", m_meshInfo.assetFilePath.c_str())))
	{
		std::string defPath_ = COMMON_ASSET_FILE_PATH;
		if (Application::Instance().GetWindow().OpenFileDialog(defPath_))
		{
			// 変更したファイルパスを取得して変数に代入し、画像をロードしなおす
			m_meshInfo.assetFilePath = defPath_;

			if (!m_mesh) { m_mesh = std::make_shared<KdModelWork>(); }
			if (m_mesh ) { m_mesh->SetModelData(defPath_);           }
		}
	}

	ImGui::ColorEdit4("Color", &m_meshInfo.color.x);
}
void ActorBase::DrawImGuiCollisionInspector()
{

}

void ActorBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	if (Json.contains("Transform")) { m_transform   = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo    = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }

	if (Json.contains("GravityInfo")) { m_gravityInfo = JsonUtility::JsonToGravityInfo(Json["GravityInfo"]); }

	m_maxMoveSpeed = Json.value("MaxMoveSpeed" , 0.0f);
}

nlohmann::json ActorBase::SaveJsonData()
{
	// TODO
	// 絶対にステータスと座標管理などは分けるべき
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["Transform"] = JsonUtility::Transform3DToJson(m_transform);
	json_["MeshInfo" ] = JsonUtility::MeshInfoToJson   (m_meshInfo );

	json_["GravityInfo"] = JsonUtility::GravityInfoToJson(m_gravityInfo);

	json_["MaxMoveSpeed"] = m_maxMoveSpeed;

	return json_;
}

void ActorBase::LoadAsset()
{
	// "Json"ファイルからパスを読み取れなければ処理止める(ファイルパスが設定されていない状態でロードすると)
	// "assert"が出るから
	if (m_meshInfo.assetFilePath.empty()) { return; }

	if (!m_mesh)
	{
		m_mesh = std::make_shared<KdModelWork>();
		m_mesh->SetModelData(m_meshInfo.assetFilePath);
	}
}

void ActorBase::UpdateGravity()
{
	const float deltaTime_ = Application::Instance().GetScaledDeltaTime();

	// 重力が最大になるまで重力加速度を足しこむ
	if(m_gravityInfo.currentGravity <= m_gravityInfo.maxGravity * deltaTime_)
	{
		m_gravityInfo.currentGravity += m_gravityInfo.accumulatedGravity * deltaTime_;
	}
}

void ActorBase::MapCollision()
{
	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	if (!scene_) { return; }

	{
		// レイのパラメータを設定
		// TODO
		m_mapCollisonRayInfo.m_pos = m_transform.location;
		m_mapCollisonRayInfo.m_pos.y += 0.13f;
		m_mapCollisonRayInfo.m_dir = -Math::Vector3::UnitY;
		m_mapCollisonRayInfo.m_range = 0.20f + m_gravityInfo.currentGravity;
		m_mapCollisonRayInfo.m_type = KdCollider::TypeGround;

		if (m_pDebugWire)
		{
			m_pDebugWire->AddDebugLine(m_mapCollisonRayInfo.m_pos, m_mapCollisonRayInfo.m_dir, m_mapCollisonRayInfo.m_range);
		}

		std::list<KdCollider::CollisionResult> resultList_;

		for(auto& obj_ : scene_->GetObjectList())
		{
			obj_->Intersects(m_mapCollisonRayInfo , &resultList_);
		}

		// レイに当たったリザルトリストから一番レイとオブジェクトの重なった量が
		// 大きいオブジェクトを探す
		Math::Vector3 hitLocation_ = Math::Vector3::Zero;
		float maxOverLapWidth      = 0.0f;
		bool  hit_                 = false;

		for(auto& result_ : resultList_)
		{
			if(maxOverLapWidth < result_.m_overlapDistance)
			{
				maxOverLapWidth = result_.m_overlapDistance;
				hitLocation_    = result_.m_hitPos;
				hit_            = true;
			}
		}

		if(hit_)
		{
			m_transform.location.y       = hitLocation_.y + m_mapCollisonRayInfo.m_range;
			m_gravityInfo.currentGravity = 0.0f;
		}
	}
}

void ActorBase::FixMatrix()
{
	// 重力分引く
	m_movement.y -= m_gravityInfo.currentGravity;
	m_transform.location += m_movement;

	const Math::Matrix transMat_    = Math::Matrix::CreateTranslation(m_transform.location);
	const Math::Matrix rotationMat_ = Math::Matrix::CreateFromYawPitchRoll
	(
		DirectX::XMConvertToRadians(m_transform.rotation.y) , 
		DirectX::XMConvertToRadians(m_transform.rotation.x) , 
		DirectX::XMConvertToRadians(m_transform.rotation.z) 
	);
	const Math::Matrix scaleMat_ = Math::Matrix::CreateScale(m_transform.scale);

	m_mWorld = scaleMat_ * rotationMat_* transMat_;
}