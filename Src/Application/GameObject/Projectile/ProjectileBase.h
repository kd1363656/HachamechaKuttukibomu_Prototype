#pragma once
#include "../../Utility/CommonStruct.h"

class ProjectileBase : public KdGameObject
{
public:

	ProjectileBase         ()         = default;
	virtual ~ProjectileBase()override = default;

	virtual uint32_t GetFinalBaseTypeID()const override{ return GameObjectID::GetTypeID<ProjectileBase>(); }

	void Init        () override;
	void PostLoadInit() override;

	void Draw() override;

	void PostUpdate() override;

	void DrawImGuiInspectors      () override;
	void DrawImGuiPrefabInspectors() override;

	void           LoadTransformData(const nlohmann::json& Json) override;
	nlohmann::json SaveTransformData()						     override;

	virtual void           LoadPrefabData(const nlohmann::json& Json) override;
	virtual nlohmann::json SavePrefabData()                           override;

	void LoadAsset() final override;

	void SetLocation(Math::Vector3& Location) { m_transform.location = Location; }

private:

	void DrawImGuiTransformInspector();
	void DrawImGuiMaterialInspector ();

	const char* PREFAB_BASE_DIRECTORY = "ProjectileBase/";

protected:

	void FixMatrix();

	std::shared_ptr<KdModelWork> m_animationMesh = nullptr;
	std::shared_ptr<KdModelData> m_staticMesh    = nullptr;

	CommonStruct::Transform3D m_transform = {};
	CommonStruct::MeshInfo    m_meshInfo  = {};

	bool m_hasAnimation = false;
};