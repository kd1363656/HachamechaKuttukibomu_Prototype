#include "KdGameObject.h"

#include "../Src/Application/Utility/StringUtility.h"
#include "../Src/System/ImGui/ImGuiManager.h"
#include "../Src/System/FileSystem/FileSystem.h"

void KdGameObject::Init()
{
	m_mWorld = Math::Matrix::Identity;

	// "Json"ファイルに登録するためだけに使用
	// 本来なら"typeid"はコンパイラ依存なので使用すべきでない(実装優先なので細かいことはしない)
	m_typeName = typeid(*this).name();
	// 名前を整形
	StringUtility::StripClassPrefix(m_typeName);

	m_isExpired = false;

	m_drawType = 0u;

	if (!m_pDebugWire) 
	{ 
		m_pDebugWire = std::make_unique<KdDebugWireFrame>(); 
	}
}
void KdGameObject::PostLoadInit()
{
	// TODO
	//const std::string fileName_ = "Asset/Data/Prefab/" + m_typeName;
	//nlohmann::json json_ = SaveJsonData();
	//FileSystem::SaveJsonFile(json_, fileName_);
}

void KdGameObject::DrawDebug()
{
	// 早期リターン
	if (!m_pDebugWire) 
	{
		return;
	}

	m_pDebugWire->Draw();
}

void KdGameObject::DrawImGuiPrefabInspectors()
{
	DrawImGuiDrawTypeInspector ();
	DrawImGuiCollisionInspector();
}

void KdGameObject::DrawImGuiDrawTypeInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("DrawType");

	// TODO
	static DrawTypeList list_[] =
	{
		{ "Lit"						  , static_cast<uint8_t>(DrawType::Lit                      ) } ,
		{ "UnLit"                     , static_cast<uint8_t>(DrawType::UnLit                    ) } ,
		{ "Effect"                    , static_cast<uint8_t>(DrawType::Effect                   ) } ,
		{ "Bright"                    , static_cast<uint8_t>(DrawType::Bright                   ) } ,
		{ "UI"                        , static_cast<uint8_t>(DrawType::UI                       ) } ,
		{ "Sprite"                    , static_cast<uint8_t>(DrawType::Sprite                   ) } ,
		{ "GenerateDepthFromMapLight" , static_cast<uint8_t>(DrawType::GenerateDepthFromMapLight) }
	};

	ImGui::PushID(list_);
	if (ImGui::BeginCombo("##Item", "Type"))
	{
		for (auto item_ : list_)
		{
			bool isSelected_ = (m_drawType & item_.type);

			if (ImGui::Checkbox(item_.label, &isSelected_))
			{
				if (isSelected_)
				{
					m_drawType |= item_.type;
				}
				else
				{
					m_drawType &= ~item_.type;
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}
void KdGameObject::DrawImGuiCollisionInspector()
{
	auto& imGui_ = ImGuiManager::GetInstance();

	imGui_.DrawSeparate();
	ImGui::Text("Collision");

	static CollisionTypeList list_[] =
	{
		{ "TypeGround"     , KdCollider::TypeGround     } ,
		{ "TypeBump"       , KdCollider::TypeBump       } ,
		{ "TypeDamage"     , KdCollider::TypeDamage     } ,
		{ "TypeDamageLine" , KdCollider::TypeDamageLine } ,
		{ "TypeSight"      , KdCollider::TypeSight      } ,
		{ "TypeEvent"      , KdCollider::TypeEvent      }
	};

	ImGui::PushID(list_);
	if (ImGui::BeginCombo("##Item", "Type"))
	{
		for (auto& item_ : list_)
		{
			// 特定の"bit"が立っていれば"true"
			// チェックボックスがついているかどうかを示すことができる("ImGui"上の表示に反映される)
			bool isSelected_ = (m_collisionType & item_.type) != 0u;

			// 第二引数の"bool"型はユーザーがクリックしたときのチェックボックスに
			// チェックがついているかどうかの状態の反対の状態を返す
			if (ImGui::Checkbox(item_.label , &isSelected_))
			{
				if (isSelected_)
				{
					m_collisionType |= item_.type;
				}
				else
				{
					m_collisionType &= ~item_.type;
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	ImGui::Text("Registered Collision");
	for (auto& item_ : list_)
	{
		if (m_collisionType & item_.type)
		{
			ImGui::Text(item_.label);
		}
	}
}

void KdGameObject::CalcDistSqrFromCamera(const Math::Vector3& camPos)
{
	m_distSqrFromCamera = (m_mWorld.Translation() - camPos).LengthSquared();
}

void KdGameObject::EnableDrawFlag(DrawType Type)
{
	const uint8_t flag_ = static_cast<uint8_t>(Type);

	m_drawType |= flag_;
}
void KdGameObject::DisableDrawFlag(DrawType Type)
{
	const uint8_t flag_ = static_cast<uint8_t>(Type);

	m_drawType &= ~flag_;
}

bool KdGameObject::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) 
	{
		return false; 
	}

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}
bool KdGameObject::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) 
	{
		return false; 
	}

	return m_pCollider->Intersects(targetBox, m_mWorld, pResults);
}
bool KdGameObject::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults) const
{
	if (!m_pCollider) 
	{
		return false; 
	}

	return m_pCollider->Intersects(targetShape, m_mWorld, pResults);
}

bool KdGameObject::HasDrawTypeFlag(DrawType Type)
{
	const uint8_t flag_ = static_cast<uint8_t>(Type);

	// フラグが立っていたら"true"を返す
	return (m_drawType & flag_) != 0;
}

Math::Vector3 KdGameObject::GetScale() const
{
	return Math::Vector3(m_mWorld.Right().Length(), m_mWorld.Up().Length(), m_mWorld.Backward().Length());
}

void KdGameObject::SetScale(float scalar)
{
	Math::Vector3 scale(scalar);

	SetScale(scale);
}

void KdGameObject::SetScale(const Math::Vector3& scale)
{
	Math::Vector3 vecX = m_mWorld.Right   ();
	Math::Vector3 vecY = m_mWorld.Up      ();
	Math::Vector3 vecZ = m_mWorld.Backward(); 
	
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();

	m_mWorld.Right   (vecX * scale.x);
	m_mWorld.Up      (vecY * scale.y);
	m_mWorld.Backward(vecZ * scale.z);
}