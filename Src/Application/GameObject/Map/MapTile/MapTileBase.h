#pragma once
#include "../../../Utility/CommonStruct.h"

class MapTileBase : public KdGameObject
{
public:

	MapTileBase         () = default;
	virtual ~MapTileBase() = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<MapTileBase>(); }

	virtual void Init        () override;
	virtual void PostLoadInit() override;

	virtual void Draw() override;

	virtual void PostUpdate() override;

	virtual void DrawImGuiInspectors        () override;
	
	void           LoadTransformData(const nlohmann::json& Json) override;
	nlohmann::json SaveTransformData()						     override;

	virtual void           LoadPrefabData(const nlohmann::json& Json) override;
	virtual nlohmann::json SavePrefabData()                           override;

private:

	void DrawImGuiTransformInspector();
	void DrawImGuiMaterialInspector ();
	void DrawImGuiFlagsInspector    ();

	const char* PREFAB_BASE_DIRECTORY = "MapTileBase/";

protected:

	void LoadAsset() final override;

	virtual void FixMatrix();

	std::shared_ptr<KdModelWork> m_animationMesh = nullptr;
	std::shared_ptr<KdModelData> m_staticMesh    = nullptr;

	CommonStruct::MeshInfo     m_meshInfo  = {};
	CommonStruct::Transform3D  m_transform = {};

	// アニメーションするかしないかで基底クラスを分けるのは無駄なのでフラグで管理
	bool m_hasAnimation = false;
};