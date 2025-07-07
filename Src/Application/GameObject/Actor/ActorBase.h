#pragma once
#include "../../Utility/CommonStruct.h"

class ActorBase : public KdGameObject
{
public:

	ActorBase()					 = default;
	virtual ~ActorBase()override = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<ActorBase>(); }

	virtual void Init        () override;
	virtual void PostLoadInit()override;
	
	virtual void DrawLit                  () override;
	virtual void GenerateDepthMapFromLight() override;

	virtual void PostUpdate() override;

	virtual void DrawImGuiInspectors        ()override;
	void		 DrawImGuiMaterialInspector ();
	void		 DrawImGuiTransformInspector();
	
	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void LoadAsset() final override;

	Math::Vector3& GetMovement() { return m_movement; }

	void SetMovement(Math::Vector3 Set) { m_movement = Set; }

private:

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

protected:

	virtual void FixMatrix();

	CommonStruct::MaterialInfo m_materialInfo = {};
	CommonStruct::Transform3D  m_transform    = {};

	Math::Vector3 m_movement      = Math::Vector3::Zero;
	Math::Vector3 m_moveDirection = Math::Vector3::Zero;

	float m_maxMoveSpeed  = 0.0f;
};