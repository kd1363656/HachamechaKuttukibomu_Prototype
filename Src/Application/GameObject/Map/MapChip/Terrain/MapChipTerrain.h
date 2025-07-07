#pragma once
#include "../MapChipBase.h"

class MapChipTerrain :  public MapChipBase
{
public:

	MapChipTerrain         ()         = default;
	virtual ~MapChipTerrain()override = default;

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<MapChipTerrain>(); }

	void Init        () override;
	void PostLoadInit() override;
};