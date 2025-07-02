#pragma once
#include "../../Utility/Struct.h"

class ActorBase : public KdGameObject
{
public:

	ActorBase()					 = default;
	virtual ~ActorBase()override = default;

	virtual uint32_t GetFinalBaseTypeID()const { return GameObjectID::GetTypeID<ActorBase>(); }

	virtual void RegisterBaseID()override;

	virtual void Init    () override;
	virtual void DrawLit () override;
	virtual void Update  () override;

protected:

	void FixMatrix();

	Tag::Transform3D m_transform = {};

	Tag::MaterialInfo m_materialInfo = {};
};