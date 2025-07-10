#pragma once
#include "../ProjectileBase.h"

class StickyBomb : public ProjectileBase
{
public:

	StickyBomb ()         = default;
	~StickyBomb()override = default;

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<StickyBomb>(); }

	void Update() override;
};