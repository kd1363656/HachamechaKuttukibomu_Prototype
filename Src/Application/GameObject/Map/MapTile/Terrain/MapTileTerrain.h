#pragma once
#include "../MapTileBase.h"

class MapTileTerrain :  public MapTileBase
{
public:

	MapTileTerrain         ()         = default;
	virtual ~MapTileTerrain()override = default;

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<MapTileTerrain>(); }

	void Init        () override;
	void PostLoadInit() override;
};