#include "MapTileBase.h"

#include "../../../Utility/JsonUtility.h"
#include "../../../Utility/CommonStruct.h"

#include "../../../main.h"

#include "../../../../System/ImGui/ImGuiManager.h"

void MapTileBase::Init()
{
	KdGameObject::Init();
	
	m_prefabSavePath += PREFAB_BASE_DIRECTORY;

	m_meshInfo  = {};
	m_transform = {};

	m_meshInfo.assetFilePath = COMMON_ASSET_FILE_PATH;

	m_hasAnimation = false;

	EnableDrawFlag(KdGameObject::DrawType::Lit);
}
void MapTileBase::PostLoadInit()
{
	LoadAsset();
	FixMatrix();
}

void MapTileBase::Draw()
{
	if (m_hasAnimation)
	{
		if (!m_animationMesh) { return; }
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_animationMesh, m_mWorld, m_meshInfo.color);
	}
	else
	{
		if (!m_staticMesh) { return; }
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_staticMesh, m_mWorld, m_meshInfo.color);
	}
}

void MapTileBase::PostUpdate()
{
	FixMatrix();
}

void MapTileBase::DrawImGuiInspectors()
{
	// 汚くなったら関数でまとめる
	DrawImGuiTransformInspector();
	DrawImGuiMaterialInspector ();
	DrawImGuiCollisionInspector();
	DrawImGuiFlagsInspector    ();

	KdGameObject::DrawImGuiInspectors();
}

void MapTileBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	if (Json.contains("Transform")) { m_transform = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo  = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }

	m_drawType      = Json.value("DrawType"      , static_cast<uint8_t>(KdGameObject::DrawType::Lit));
	m_collisionType = Json.value("CollisionType" , static_cast<uint8_t>(KdCollider::TypeGround     ));

	m_hasAnimation = Json.value("HasAnimation" , false);
}
nlohmann::json MapTileBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["Transform"] = JsonUtility::Transform3DToJson(m_transform);
	json_["MeshInfo" ] = JsonUtility::MeshInfoToJson   (m_meshInfo );

	json_["DrawType"     ] = m_drawType;
	json_["CollisionType"] = m_collisionType;

	json_["HasAnimation"] = m_hasAnimation;

	return json_;
}

void MapTileBase::DrawImGuiTransformInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");

	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}
void MapTileBase::DrawImGuiMaterialInspector()
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

			if(m_hasAnimation)
			{
				if (!m_animationMesh)
				{
					m_animationMesh = std::make_shared<KdModelWork>();
				}
			}
			else
			{
				if(!m_staticMesh)
				{
					m_staticMesh = std::make_shared<KdModelData>();
				}
			}

			if (m_staticMesh)
			{
				m_staticMesh->Load(defPath_);
			}
		}
	}

	ImGui::ColorEdit4("Color", &m_meshInfo.color.x);
}
void MapTileBase::DrawImGuiFlagsInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Flags");
	ImGui::Checkbox("HasAnimation", &m_hasAnimation);
}

void MapTileBase::LoadAsset()
{
	// ファイルパスが存在しなければ"return"
	if (m_meshInfo.assetFilePath.empty()) { return; }

	if (m_hasAnimation && !m_animationMesh)
	{
		m_animationMesh = std::make_shared<KdModelWork>();
		m_animationMesh->SetModelData(m_meshInfo.assetFilePath);
	}
	else if (!m_hasAnimation && !m_staticMesh)
	{
		m_staticMesh = std::make_shared<KdModelData>();
		m_staticMesh->Load(m_meshInfo.assetFilePath);
	}
}

void MapTileBase::FixMatrix()
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