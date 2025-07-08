#include "PlayerMoveState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

#include "../Idle/PlayerIdleState.h"
#include "../StickyBombThrow/PlayerPrepareStickyBombThrowState.h"

void PlayerMoveState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Move\n");
}

void PlayerMoveState::Update(Player* Owner)
{
	const bool isPrepareStickyBombThrow_ = Owner->IsStickyBombThrowKeyPressed();

	Owner->Move();

	// TODO
	if(isPrepareStickyBombThrow_ && !ImGui::GetIO().WantCaptureMouse)
	{
		m_stateMachine->ChangeState<PlayerPrepareStickyBombThrowState>();
	}
	if(Owner->GetMovement() == Math::Vector3::Zero)
	{
		m_stateMachine->ChangeState<PlayerIdleState>();
	}
}