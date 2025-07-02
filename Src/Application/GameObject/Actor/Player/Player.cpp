#include "Player.h"

void Player::Init()
{
	ActorBase::Init();

	m_materialInfo.assetFilePath = "Asset/Models/Player/MF_IDLE/MF_IDLE.gltf";

	if(!m_materialInfo.modelWork) { m_materialInfo.modelWork = std::make_shared<KdModelWork>();			  }
	if (m_materialInfo.modelWork) { m_materialInfo.modelWork->SetModelData(m_materialInfo.assetFilePath); }
}