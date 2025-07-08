#include "PlayerMoveState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

#include "../Idle/PlayerIdleState.h"

void PlayerMoveState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Move\n");
}

void PlayerMoveState::Update(Player* Owner)
{
	Owner->Move();

	if(Owner->GetMovement() == Math::Vector3::Zero)
	{
		m_stateMachine->ChangeState<PlayerIdleState>();
	}
}