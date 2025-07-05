#pragma once
#include "../../Utility/Struct.h"

class ActorBase : public KdGameObject
{
public:

	ActorBase()					 = default;
	virtual ~ActorBase()override = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<ActorBase>(); }

	virtual void Init    () override;
	virtual void DrawLit () override;
	virtual void Update  () override;

	virtual void ImGuiMaterialInspector ()override;
	virtual void ImGuiTransformInspector()override;

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

private:

	const std::string COMMON_ASSET_FILE_PATH = "Asset/";

protected:

	void FixMatrix();

	Tag::Transform3D m_transform = {};

	Tag::MaterialInfo m_materialInfo = {};
};