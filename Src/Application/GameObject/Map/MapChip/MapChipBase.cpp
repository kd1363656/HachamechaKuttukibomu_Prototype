#include "MapChipBase.h"

#include "../../../Utility/JsonUtility.h"
#include "../../../Utility/ImGui/ImGuiManager.h"

#include "../../../main.h"

void MapChipBase::Init()
{
	KdGameObject::Init();
	
	m_materialInfo = {};
	m_transform    = {};

	m_materialInfo.assetFilePath = COMMON_ASSET_FILE_PATH;
}

void MapChipBase::PostLoadInit()
{
	LoadAsset();
	FixMatrix();
}

void MapChipBase::DrawLit()
{
	if (!m_materialInfo.modelWork) { return; }

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_materialInfo.modelWork , m_mWorld , m_materialInfo.color);
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
void MapChipBase::DrawImGuiMaterialInspector()
{
	if (ImGui::Button(("TextureFilePath : %s", m_materialInfo.assetFilePath.c_str())))
	{
		std::string defPath_ = COMMON_ASSET_FILE_PATH;
		if (Application::Instance().GetWindow().OpenFileDialog(defPath_))
		{
			// 変更したファイルパスを取得して変数に代入し、画像をロードしなおす
			m_materialInfo.assetFilePath = defPath_;

			if (!m_materialInfo.modelWork)
			{
				m_materialInfo.modelWork = std::make_shared<KdModelWork>();
			}

			if (m_materialInfo.modelWork)
			{
				m_materialInfo.modelWork->SetModelData(defPath_);
			}
		}
	}

	ImGui::ColorEdit4("Color", &m_materialInfo.color.x);
}
void MapChipBase::DrawImGuiTransformInspector()
{
	ImGui::DragFloat3("Location" , &m_transform.location.x , 0.1f);
	ImGui::DragFloat3("Rotation" , &m_transform.rotation.x , 1.0f);
	ImGui::DragFloat3("Scale"    , &m_transform.scale.x    , 0.1f);
}
void MapChipBase::DrawImGuiCollisionInspector()
{
	// TODO
	// オーバーヘッド気になるなら"static"にする
	CollisionTypeList list_[] =
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
		bool selected_ = (m_collisionType & item_.Type) != 0u;

		// 第二引数の"bool"型はユーザーがクリックしたときのチェックボックスに
		// チェックがついているかどうかの状態のフラグを返す
		if (ImGui::Checkbox(item_.Label , &selected_))
		{
			if (selected_)
			{
				m_collisionType |= item_.Type;
			}
			else
			{
				m_collisionType &= ~item_.Type;
			}
		}
	}

	ImGui::Text("Registerd Collision");
	for (auto& item_ : list_)
	{
		if (m_collisionType & item_.Type)
		{
			ImGui::Text(item_.Label);
		}
	}
}

void MapChipBase::LoadJsonData(const nlohmann::json Json)
{
	// Jsonで設定した値を代入
	m_typeName = Json.value("TypeName" , "");

	m_materialInfo.assetFilePath = Json.value("AssetFilePath" , "");
	m_materialInfo.color         = JsonUtility::JsonToVec4(Json["Color"]);

	m_transform.scale    = JsonUtility::JsonToVec3(Json["Scale"]);
	m_transform.rotation = JsonUtility::JsonToVec3(Json["Rotation"]);
	m_transform.location = JsonUtility::JsonToVec3(Json["Location"]);

	m_collisionType = Json.value("CollisionType" , 0u);
}

nlohmann::json MapChipBase::SaveJsonData()
{
	nlohmann::json json_;

	json_["TypeName"] = m_typeName;

	json_["AssetFilePath"] = m_materialInfo.assetFilePath;
	json_["Color"]         = JsonUtility::Vec4ToJson(m_materialInfo.color);

	json_["Scale"]    = JsonUtility::Vec3ToJson(m_transform.scale);
	json_["Rotation"] = JsonUtility::Vec3ToJson(m_transform.rotation);
	json_["Location"] = JsonUtility::Vec3ToJson(m_transform.location);

	json_["CollisionType"] = m_collisionType;

	return json_;
}

void MapChipBase::LoadAsset()
{
	if (m_materialInfo.assetFilePath.empty())
	{
		return;
	}

	if (!m_materialInfo.modelWork)
	{
		m_materialInfo.modelWork = std::make_shared<KdModelWork>();
		m_materialInfo.modelWork->SetModelData(m_materialInfo.assetFilePath);
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