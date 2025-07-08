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

	m_mapRayColliderSetting = {};

	m_meshInfo.assetFilePath = COMMON_ASSET_FILE_PATH;

	m_isInAir = false;
}

void ActorBase::PostLoadInit()
{
	LoadAsset();

	FixMatrix ();
	PostUpdate();
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
	MapCollision();
	FixMatrix   ();
}

void ActorBase::DrawImGuiInspectors()
{
	DrawImGuiTransformInspector();
	DrawImGuiMaterialInspector ();
	DrawImGuiCollisionInspector();
}
void ActorBase::DrawImGuiTransformInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");

	ImGui::DragFloat3("Location", &m_transform.location.x, 0.1f);
	ImGui::DragFloat3("Rotation", &m_transform.rotation.x, 1.0f);
	ImGui::DragFloat3("Scale"   , &m_transform.scale.x   , 0.1f);
}
void ActorBase::DrawImGuiMaterialInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Material");

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
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Collision");

	ImGui::Text("m_isInAir : %d" , m_isInAir);
}

void ActorBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	if (Json.contains("Transform")) { m_transform   = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo    = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }

	// TODO
	if (Json.contains("GravityInfo")) { m_gravityInfo = JsonUtility::JsonToGravityInfo(Json["GravityInfo"]); }

	if (Json.contains("MapRayColliderSetting"   )) { m_mapRayColliderSetting    = JsonUtility::JsonToRayColliderSetting   (Json["MapRayColliderSetting"   ]); }
	if (Json.contains("MapSphereColliderSetting")) { m_mapSphereColliderSetting = JsonUtility::JsonToSphereColliderSetting(Json["MapSphereColliderSetting"]); }

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

	// TODO
	json_["GravityInfo"] = JsonUtility::GravityInfoToJson(m_gravityInfo);
	json_["MaxMoveSpeed"] = m_maxMoveSpeed;

	json_["MapRayColliderSetting"   ] = JsonUtility::RayColliderSettingToJson   (m_mapRayColliderSetting   );
	json_["MapSphereColliderSetting"] = JsonUtility::SphereColliderSettingToJson(m_mapSphereColliderSetting);

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

		// レイ判定
	{
		// レイのパラメータを設定
		// TODO
		KdCollider::RayInfo rayInfo_ = {};

		rayInfo_.m_pos   = m_transform.location + m_movement;
		rayInfo_.m_pos  += m_mapRayColliderSetting.offset;
		rayInfo_.m_dir   = m_mapRayColliderSetting.direction;
		rayInfo_.m_range = m_mapRayColliderSetting.range + m_gravityInfo.currentGravity;
		rayInfo_.m_type  = m_mapRayColliderSetting.collisionType;

		if (m_pDebugWire)
		{
			m_pDebugWire->AddDebugLine(rayInfo_.m_pos, rayInfo_.m_dir, rayInfo_.m_range);
		}

		std::list<KdCollider::CollisionResult> resultList_;

		for(auto& obj_ : scene_->GetObjectList())
		{
			obj_->Intersects(rayInfo_ , &resultList_);
		}

		// レイに当たったリザルトリストから一番レイとオブジェクトの重なった量が
		// 大きいオブジェクトを探す
		Math::Vector3 hitLocation_     = Math::Vector3::Zero;
		float         maxOverLapWidth_ = 0.0f;
		bool          hit_             = false;

		for(auto& result_ : resultList_)
		{
			if(maxOverLapWidth_ < result_.m_overlapDistance)
			{
				maxOverLapWidth_ = result_.m_overlapDistance;
				hitLocation_     = result_.m_hitPos;
				hit_             = true;
			}
		}

		if(hit_)
		{
			m_transform.location.y       = hitLocation_.y;
			m_gravityInfo.currentGravity = 0.0f;
			m_isInAir					 = false;
		}
		else
		{
			m_isInAir = true;
		}
	}

	// 球判定
	{
		KdCollider::SphereInfo sphereInfo_ = {};

		sphereInfo_.m_sphere.Center    = m_transform.location + m_movement;
		sphereInfo_.m_sphere.Center.y += m_mapSphereColliderSetting.offset.y;

		sphereInfo_.m_sphere.Radius = m_mapSphereColliderSetting.radius;
		sphereInfo_.m_type          = m_mapSphereColliderSetting.collisionType;

		m_pDebugWire->AddDebugSphere(sphereInfo_.m_sphere.Center , sphereInfo_.m_sphere.Radius);

		std::list<KdCollider::CollisionResult> resultList_;

		for(auto& obj_ : scene_->GetObjectList())
		{
			obj_->Intersects(sphereInfo_, &resultList_);
		}

		Math::Vector3 hitDirection_    = Math::Vector3::Zero;
		float         maxOverLapWidth_ = 0.0f;
		bool          hit_             = false;

		for(auto& result_ : resultList_)
		{
			if(maxOverLapWidth_ < result_.m_overlapDistance)
			{
				maxOverLapWidth_ = result_.m_overlapDistance;
				hitDirection_    = result_.m_hitDir;
				hit_             = true;
			}
		}

		if(hit_)
		{
			hitDirection_.Normalize();

			m_transform.location        += hitDirection_ * maxOverLapWidth_;
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