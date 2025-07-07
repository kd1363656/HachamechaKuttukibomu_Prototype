#include "MapChipTerrain.h"

void MapChipTerrain::Init()
{
	MapChipBase::Init();
}

void MapChipTerrain::PostLoadInit()
{
	MapChipBase::PostLoadInit();

	if (!m_pCollider)
	{
		m_pCollider = std::make_unique<KdCollider>();

		m_pCollider->RegisterCollisionShape
		(
			{ "MapChipTerrain"         } ,
			{ m_mesh                   } ,
			{ m_collisionType          }
		);
	}
}