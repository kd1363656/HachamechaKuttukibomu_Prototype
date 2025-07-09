#include "MapTileTerrain.h"

void MapTileTerrain::Init()
{
	MapTileBase::Init();
}

void MapTileTerrain::PostLoadInit()
{
	MapTileBase::PostLoadInit();

	if (m_pCollider) { return; }

	m_pCollider = std::make_unique<KdCollider>();

	if (m_hasAnimation && m_animationMesh)
	{
		m_pCollider->RegisterCollisionShape
		(
			{ "MapTileTerrain" } ,
			{ m_animationMesh  } ,
			{ m_collisionType  }
		);
	}
	else if (!m_hasAnimation && m_staticMesh)
	{
		m_pCollider->RegisterCollisionShape
		(
			{ "MapTileTerrain" } ,
			{ m_staticMesh     } ,
			{ m_collisionType  }
		);	
	}
}