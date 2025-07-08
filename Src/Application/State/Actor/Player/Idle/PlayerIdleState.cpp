#include "PlayerIdleState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

#include "../Move/PlayerMoveState.h"

void PlayerIdleState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Idle\n");
}

void PlayerIdleState::Update(Player* Owner)
{
	bool isMove_                   = Owner->IsMoveKeyPressed();
	bool isPrepareStickyBombThrow_ = Owner->IsStickyBombThrowKeyPressed();

	if(isMove_) 
	{ 
		m_stateMachine->ChangeState<PlayerMoveState>(); 
	}
}