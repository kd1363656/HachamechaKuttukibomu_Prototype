#include "ActorBase.h"

#include "../../Factory/Factory.h"

#include "../../Utility/JsonUtility.h"

#include "../../main.h"

void ActorBase::Init()
{
	KdGameObject::Init();
	
	m_materialInfo = {};
	m_transform    = {};

	m_moveDirection = Math::Vector3::Zero;
	m_movement      = Math::Vector3::Zero;
}

void ActorBase::DrawUnLit()
{
	if (!m_materialInfo.modelWork) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_materialInfo.modelWork , m_mWorld , m_materialInfo.color);
}

void ActorBase::GenerateDepthMapFromLight()
{
	if (!m_materialInfo.modelWork) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_materialInfo.modelWork, m_mWorld, m_materialInfo.color);
}

void ActorBase::PostUpdate()
{
	FixMatrix();
}

void ActorBase::ImGuiMaterialInspector()
{
	if (ImGui::Button(("TextureFilePath : %s", m_materialInfo.assetFilePath.c_str())))
	{
		std::string defPath_ = COMMON_ASSET_FILE_PATH;
		if (Application::Instance().GetWindow().OpenFileDialog(defPath_))
		{
			// 変更したファイルパスを取得して変数に代入し、画像をロードしなおす
			m_materialInfo.assetFilePath = defPath_;

			if (m_materialInfo.modelWork)
			{
				m_materialInfo.modelWork->SetModelData(defPath_);
			}
		}
	}

	ImGui::ColorEdit4("Color", &m_materialInfo.color.x);
}
void ActorBase::ImGuiTransformInspector()
{
	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}

void ActorBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	m_materialInfo.assetFilePath = Json.value("AssetFilePath" , "");
	m_materialInfo.color         = JsonUtility::JsonToVec4(Json["Color"]);

	m_transform.scale    = JsonUtility::JsonToVec3(Json["Scale"]);
	m_transform.rotation = JsonUtility::JsonToVec3(Json["Rotation"]);
	m_transform.location = JsonUtility::JsonToVec3(Json["Location"]);

	m_maxMoveSpeed = Json.value("MaxMoveSpeed" , 0.0f);
}

nlohmann::json ActorBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["AssetFilePath"] = m_materialInfo.assetFilePath;
	json_["Color"]         = JsonUtility::Vec4ToJson(m_materialInfo.color);

	json_["Scale"]    = JsonUtility::Vec3ToJson(m_transform.scale);
	json_["Rotation"] = JsonUtility::Vec3ToJson(m_transform.rotation);
	json_["Location"] = JsonUtility::Vec3ToJson(m_transform.location);

	json_["MaxMoveSpeed"] = m_maxMoveSpeed;

	return json_;
}

void ActorBase::FixMatrix()
{
	const Math::Matrix transMat_    = Math::Matrix::CreateTranslation     (m_transform.location);
	const Math::Matrix rotationMat_ = Math::Matrix::CreateFromYawPitchRoll(m_transform.rotation);
	const Math::Matrix scaleMat_    = Math::Matrix::CreateScale           (m_transform.scale   );

	m_mWorld = transMat_ * rotationMat_ * scaleMat_;
}