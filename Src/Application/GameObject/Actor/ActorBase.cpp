#include "ActorBase.h"

#include "../../Factory/Factory.h"

#include "../../Utility/JsonUtility.h"
#include "../../Utility/ImGui/ImGuiManager.h"

#include "../../main.h"

void ActorBase::Init()
{
	KdGameObject::Init();
	
	m_meshInfo  = {};
	m_transform = {};

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
	if (!m_skeletonMesh) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_skeletonMesh, m_mWorld , m_meshInfo.color);
}

void ActorBase::GenerateDepthMapFromLight()
{
	if (!m_skeletonMesh) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_skeletonMesh, m_mWorld, m_meshInfo.color);
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

			if(!m_skeletonMesh)
			{
				m_skeletonMesh = std::make_shared<KdModelWork>();
			}

			if (m_skeletonMesh)
			{
				m_skeletonMesh->SetModelData(defPath_);
			}
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

	if (Json.contains("Transform")) { m_transform = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo  = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }

	m_maxMoveSpeed = Json.value("MaxMoveSpeed" , 0.0f);
}

nlohmann::json ActorBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["Transform"] = JsonUtility::Transform3DToJson(m_transform);
	json_["MeshInfo" ] = JsonUtility::MeshInfoToJson   (m_meshInfo);

	json_["MaxMoveSpeed"] = m_maxMoveSpeed;

	return json_;
}

void ActorBase::LoadAsset()
{
	// "Json"ファイルからパスを読み取れなければ処理止める(ファイルパスが設定されていない状態でロードすると)
	// "assert"が出るから
	if (m_meshInfo.assetFilePath.empty())return;

	if (!m_skeletonMesh)
	{
		m_skeletonMesh = std::make_shared<KdModelWork>();
		m_skeletonMesh->SetModelData(m_meshInfo.assetFilePath);
	}
}

void ActorBase::MapCollision()
{
	

}

void ActorBase::FixMatrix()
{
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