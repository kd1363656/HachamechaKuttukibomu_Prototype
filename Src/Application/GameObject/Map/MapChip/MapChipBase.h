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

	bool CheckInScreen(const DirectX::BoundingFrustum& Frustum) const final override;

	virtual void DrawImGuiInspectors        ()override;
	void		 DrawImGuiMaterialInspector ();
	void		 DrawImGuiTransformInspector();
	void		 DrawImGuiCollisionInspector();

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void LoadAsset() final override;

private:

	// "ImGui"で使うだけのもの
	struct CollisionTypeList
	{
		const char* Label;
		uint32_t    Type;
	};

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

protected:

	virtual void FixMatrix();

	CommonStruct::MaterialInfo m_materialInfo = {};
	CommonStruct::Transform3D  m_transform    = {};

	uint32_t m_collisionType = 0u;
};