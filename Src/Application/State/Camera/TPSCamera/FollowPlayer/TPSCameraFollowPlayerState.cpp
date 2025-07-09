#include "TPSCameraFollowPlayerState.h"
#include "../PlayerThrowFocus/TPSCameraFocusOnPlayerThrowState.h"

#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../../../GameObject/Actor/Player/Player.h"

void TPSCameraFollowPlayerState::Update(TPSCamera* Owner)
{
	Owner->UpdateRotateByMouse();
	
	Owner->LerpToStandardCameraLocation();

	if (Owner->HasStateFlag(TPSCamera::StateFlg::PlayerThrowFocus))
	{
		m_stateMachine->ChangeState<TPSCameraFocusOnPlayerThrowState>();
	}
}