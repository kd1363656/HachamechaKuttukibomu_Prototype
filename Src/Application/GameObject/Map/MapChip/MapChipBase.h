#pragma once
#include "../../../Utility/CommonStruct.h"

class MapChipBase : public KdGameObject
{
public:

	MapChipBase         () = default;
	virtual ~MapChipBase() = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<MapChipBase>(); }

	virtual void Init        () override;
	virtual void PostLoadInit() override;

	virtual void DrawLit() override;

	virtual void PostUpdate() override;

	virtual void DrawImGuiInspectors        () override;
	void		 DrawImGuiTransformInspector();
	void		 DrawImGuiMaterialInspector ();
	void		 DrawImGuiCollisionInspector();

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void LoadAsset() final override;

private:

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

protected:

	virtual void FixMatrix();

	std::shared_ptr<KdModelWork> m_mesh = nullptr;

	CommonStruct::MeshInfo     m_meshInfo  = {};
	CommonStruct::Transform3D  m_transform = {};

	uint32_t m_collisionType = 0u;
};