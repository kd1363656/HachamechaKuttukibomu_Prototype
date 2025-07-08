#include "PlayerPrepareStickyBombThrowState.h"
#include "PlayerStickyBombThrowState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

void PlayerPrepareStickyBombThrowState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : PrepareStickyBombThrowState\n");
	m_isThrowStickyBombKeyReleased = false;
}

void PlayerPrepareStickyBombThrowState::Update(Player* Owner)
{
	const bool isPrepareStickyBombThrow_ = Owner->IsStickyBombThrowKeyPressed();

	Owner->PrepareStickyBombThrow();

	// TODO
	if(isPrepareStickyBombThrow_ && !ImGui::GetIO().WantCaptureMouse)
	{
		if (m_isThrowStickyBombKeyReleased)
		{
			m_stateMachine->ChangeState<PlayerStickyBombThrowState>();
		}
	}
	else
	{
		m_isThrowStickyBombKeyReleased = true;
	}
}