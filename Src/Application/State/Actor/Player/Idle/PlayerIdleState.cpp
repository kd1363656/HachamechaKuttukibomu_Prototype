#include "PlayerIdleState.h"
#include "../Move/PlayerMoveState.h"
#include "../StickyBombThrow/PlayerPrepareStickyBombThrowState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

void PlayerIdleState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Idle\n");
}

void PlayerIdleState::Update(Player* Owner)
{
	const bool isMove_                   = Owner->IsMoveKeyPressed           ();
	const bool isPrepareStickyBombThrow_ = Owner->IsStickyBombThrowKeyPressed();

	// TODO
	if(isPrepareStickyBombThrow_ && !ImGui::GetIO().WantCaptureMouse)
	{
		m_stateMachine->ChangeState<PlayerPrepareStickyBombThrowState>();
	}
	if(isMove_) 
	{ 
		m_stateMachine->ChangeState<PlayerMoveState>(); 
	}
}