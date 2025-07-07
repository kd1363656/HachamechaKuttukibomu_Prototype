#include "MapChipBase.h"

#include "../../../Utility/JsonUtility.h"
#include "../../../Utility/ImGui/ImGuiManager.h"
#include "../../../Utility/CommonStruct.h"

#include "../../../main.h"

void MapChipBase::Init()
{
	KdGameObject::Init();
	
	m_meshInfo  = {};
	m_transform = {};

	m_meshInfo.assetFilePath = COMMON_ASSET_FILE_PATH;
}

void MapChipBase::PostLoadInit()
{
	LoadAsset();
	FixMatrix();
}

void MapChipBase::DrawLit()
{
	if (!m_mesh) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_mesh , m_mWorld , m_meshInfo.color);
}

void MapChipBase::PostUpdate()
{
	FixMatrix();
}

void MapChipBase::DrawImGuiInspectors()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Transform");
	DrawImGuiTransformInspector();

	imGui_.DrawSeparate();
	ImGui::Text("Material");
	DrawImGuiMaterialInspector ();

	imGui_.DrawSeparate();
	ImGui::Text("Collision");
	DrawImGuiCollisionInspector();
}
void MapChipBase::DrawImGuiTransformInspector()
{
	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}
void MapChipBase::DrawImGuiMaterialInspector()
{
	if (ImGui::Button(("TextureFilePath : %s", m_meshInfo.assetFilePath.c_str())))
	{
		std::string defPath_ = COMMON_ASSET_FILE_PATH;
		if (Application::Instance().GetWindow().OpenFileDialog(defPath_))
		{
			// 変更したファイルパスを取得して変数に代入し、画像をロードしなおす
			m_meshInfo.assetFilePath = defPath_;

			if (!m_mesh)
			{
				m_mesh = std::make_shared<KdModelWork>();
			}

			if (m_mesh)
			{
				m_mesh->SetModelData(defPath_);
			}
		}
	}

	ImGui::ColorEdit4("Color", &m_meshInfo.color.x);
}
void MapChipBase::DrawImGuiCollisionInspector()
{
	// TODO
	CommonStruct::CollisionTypeList list_[] =
	{
		{ "TypeGround"     , KdCollider::TypeGround     } ,
		{ "TypeBump"       , KdCollider::TypeBump       } ,
		{ "TypeDamage"     , KdCollider::TypeDamage     } ,
		{ "TypeDamageLine" , KdCollider::TypeDamageLine } ,
		{ "TypeSight"      , KdCollider::TypeSight      } ,
		{ "TypeEvent"      , KdCollider::TypeEvent      }
	};

	for (auto& item_ : list_)
	{
		// 特定の"bit"が立っていれば"true"
		// チェックボックスがついているかどうかを示すことができる("ImGui"上の表示に反映される)
		bool selected_ = (m_collisionType & item_.type) != 0u;

		// 第二引数の"bool"型はユーザーがクリックしたときのチェックボックスに
		// チェックがついているかどうかの状態のフラグを返す
		if (ImGui::Checkbox(item_.label , &selected_))
		{
			if (selected_)
			{
				m_collisionType |= item_.type;
			}
			else
			{
				m_collisionType &= ~item_.type;
			}
		}
	}

	ImGui::Text("Registered Collision");
	for (auto& item_ : list_)
	{
		if (m_collisionType & item_.type)
		{
			ImGui::Text(item_.label);
		}
	}
}

void MapChipBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	if (Json.contains("Transform")) { m_transform = JsonUtility::JsonToTransform3D(Json["Transform"]); }
	if (Json.contains("MeshInfo" )) { m_meshInfo  = JsonUtility::JsonToMeshInfo   (Json["MeshInfo" ]); }

	m_collisionType = Json.value("CollisionType" , 0u);
}

nlohmann::json MapChipBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["Transform"] = JsonUtility::Transform3DToJson(m_transform);
	json_["MeshInfo" ] = JsonUtility::MeshInfoToJson   (m_meshInfo );

	json_["CollisionType"] = m_collisionType;

	return json_;
}

void MapChipBase::LoadAsset()
{
	if (m_meshInfo.assetFilePath.empty()) { return; }

	if (!m_mesh)
	{
		m_mesh = std::make_shared<KdModelWork>();
		m_mesh->SetModelData(m_meshInfo.assetFilePath);
	}
}

void MapChipBase::FixMatrix()
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