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
	
	virtual void Draw() override;

	virtual void PostUpdate() override;

	virtual void DrawImGuiInspectors()override;
	
	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void LoadAsset() final override;

	void UpdateGravity();
	void MapCollision ();

	Math::Vector3& GetMovement() { return m_movement; }

	void SetMovement(Math::Vector3 Set) { m_movement = Set; }

private:

	void DrawImGuiTransformInspector();
	void DrawImGuiMaterialInspector ();
	void DrawImGuiCollisionInspector();

protected:

	virtual void FixMatrix();
	
	std::shared_ptr<KdModelWork> m_mesh;

	CommonStruct::Transform3D m_transform   = {};
	CommonStruct::MeshInfo    m_meshInfo    = {};
	CommonStruct::GravityInfo m_gravityInfo = {};

	CommonStruct::SphereColliderSetting m_mapSphereColliderSetting = {};
	CommonStruct::RayColliderSetting    m_mapRayColliderSetting    = {};

	Math::Vector3 m_movement      = Math::Vector3::Zero;
	Math::Vector3 m_moveDirection = Math::Vector3::Zero;

	float m_maxMoveSpeed  = 0.0f;
};