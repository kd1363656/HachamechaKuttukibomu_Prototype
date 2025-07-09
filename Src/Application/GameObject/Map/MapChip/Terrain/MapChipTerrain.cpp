#include "MapChipTerrain.h"

void MapChipTerrain::Init()
{
	MapChipBase::Init();
}

void MapChipTerrain::PostLoadInit()
{
	MapChipBase::PostLoadInit();

	if (m_pCollider) { return; }

	m_pCollider = std::make_unique<KdCollider>();

	if (m_hasAnimation && m_animationMesh)
	{
		m_pCollider->RegisterCollisionShape
		(
			{ "MapChipTerrain" } ,
			{ m_animationMesh  } ,
			{ m_collisionType  }
		);
	}
	else if (!m_hasAnimation && m_staticMesh)
	{
		m_pCollider->RegisterCollisionShape
		(
			{ "MapChipTerrain" } ,
			{ m_staticMesh     } ,
			{ m_collisionType  }
		);	
	}
}