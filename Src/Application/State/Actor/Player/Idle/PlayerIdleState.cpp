#include "PlayerIdleState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

#include "../Move/PlayerMoveState.h"

void PlayerIdleState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Idle\n");
}

void PlayerIdleState::Update(Player* Owner)
{
	bool isMove_ = Owner->IsMoveKeyPressed();

	if(isMove_) 
	{ 
		m_stateMachine->ChangeState<PlayerMoveState>(); 
	}
}

void PlayerIdleState::Exit(Player* Owner)
{

}
