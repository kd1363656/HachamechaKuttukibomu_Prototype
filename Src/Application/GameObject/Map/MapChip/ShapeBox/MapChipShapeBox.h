#pragma once
#include "../MapChipBase.h"

class MapChipShapeBox :  public MapChipBase
{
public:

	MapChipShapeBox         () = default;
	virtual ~MapChipShapeBox() = default;

	void Init() override;
};