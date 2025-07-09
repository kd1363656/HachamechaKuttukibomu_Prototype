#include "BackGround.h"

#include "../../../Utility/JsonUtility.h"

#include "../../../main.h"

#include "../../../Utility/ImGui/ImGuiManager.h"

void BackGround::Init()
{
	KdGameObject::Init();

	m_meshInfo  = {};
	m_transform = {};

	m_meshInfo.assetFilePath = COMMON_ASSET_FILE_PATH;

	EnableDrawFlag(KdGameObject::DrawType::UnLit);
}

void BackGround::PostLoadInit()
{
	LoadAsset();
}

void BackGround::Draw()
{
	if (!m_staticMesh) { return; }
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_staticMesh, m_mWorld, m_meshInfo.color);
}

void BackGround::PostUpdate()
{
	FixMatrix();
}

void BackGround::DrawImGuiInspectors()
{
	DrawImGuiTransformInspector();
	DrawImGuiMaterialInspector ();

	KdGameObject::DrawImGuiInspectors();
}

void BackGround::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	if (Json.contains("Transform")) { m_transform = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo  = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }
}

nlohmann::json BackGround::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["Transform"] = JsonUtility::Transform3DToJson(m_transform);
	json_["MeshInfo" ] = JsonUtility::MeshInfoToJson   (m_meshInfo );

	return json_;
}

void BackGround::DrawImGuiTransformInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");

	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}
void BackGround::DrawImGuiMaterialInspector()
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

			if(!m_staticMesh)
			{
				m_staticMesh = std::make_shared<KdModelData>();
			}
		
			if (m_staticMesh)
			{
				m_staticMesh->Load(defPath_);
			}
		}
	}

	ImGui::ColorEdit4("Color", &m_meshInfo.color.x);
}

void BackGround::LoadAsset()
{
	// ファイルパスが存在しなければ"return"
	if (m_meshInfo.assetFilePath.empty()) { return; }

	if (!m_staticMesh)
	{
		m_staticMesh = std::make_shared<KdModelData>();
		m_staticMesh->Load(m_meshInfo.assetFilePath);
	}
}

void BackGround::FixMatrix()
{
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