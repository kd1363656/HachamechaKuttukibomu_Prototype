#pragma once
#include "../../../Utility/CommonStruct.h"

class BackGround :  public KdGameObject
{
public:

	BackGround () = default;
	~BackGround() = default;

	uint32_t GetTypeID         () const override { return GameObjectID::GetTypeID<BackGround>(); }
	uint32_t GetFinalBaseTypeID() const override { return GameObjectID::GetTypeID<BackGround>(); }

	void Init        () override;
	void PostLoadInit() override;

	void Draw() override;

	void PostUpdate() override;

	virtual void DrawImGuiInspectors() override;

	void           LoadJsonData(const nlohmann::json& Json);
	nlohmann::json SaveJsonData();

private:

	void DrawImGuiTransformInspector();
	void DrawImGuiMaterialInspector ();

	const char* PREFAB_BASE_DIRECTORY = "BackGround/";

	std::shared_ptr<KdModelData> m_staticMesh = nullptr;

	CommonStruct::MeshInfo     m_meshInfo  = {};
	CommonStruct::Transform3D  m_transform = {};

protected:

	void LoadAsset()final override;

	virtual void FixMatrix();
};