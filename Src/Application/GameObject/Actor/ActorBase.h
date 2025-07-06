#pragma once
#include "../../Utility/CommonStruct.h"

class ActorBase : public KdGameObject
{
public:

	ActorBase()					 = default;
	virtual ~ActorBase()override = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<ActorBase>(); }

	virtual void Init() override;
	
	virtual void DrawUnLit                () override;
	virtual void GenerateDepthMapFromLight() override;

	virtual void PostUpdate() override;

	virtual void ImGuiMaterialInspector () override;
	virtual void ImGuiTransformInspector() override;

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	Math::Vector3& GetMoveDirection() { return m_moveDirection; }
	Math::Vector3& GetMovement     () { return m_movement;      }

	void SetMoveDirection(Math::Vector3 Set) { m_moveDirection = Set; }
	void SetMovement     (Math::Vector3 Set) { m_movement      = Set; }

private:

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

protected:

	virtual void FixMatrix();

	Tag::MaterialInfo m_materialInfo = {};
	Tag::Transform3D  m_transform    = {};

	Math::Vector3 m_moveDirection = Math::Vector3::Zero;
	Math::Vector3 m_movement      = Math::Vector3::Zero;

	float m_maxMoveSpeed  = 0.0f;
};