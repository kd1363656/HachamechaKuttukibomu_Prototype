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

	void DrawImGuiInspectors()override;

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void LoadAsset() final override;

	void SetLocation(Math::Vector3& Location) { m_transform.location = Location; }

private:

	void DrawImGuiTransformInspector();
	void DrawImGuiMaterialInspector ();

protected:

	void FixMatrix();

	std::shared_ptr<KdModelWork> m_animationMesh = nullptr;
	std::shared_ptr<KdModelData> m_staticMesh    = nullptr;

	CommonStruct::Transform3D m_transform = {};
	CommonStruct::MeshInfo    m_meshInfo  = {};

	bool m_hasAnimation = false;
};