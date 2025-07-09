#include "TPSCameraFocusOnPlayerThrowState.h"
#include "../FollowPlayer/TPSCameraFollowPlayerState.h"

#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"

void TPSCameraFocusOnPlayerThrowState::Update(TPSCamera* Owner)
{
	Owner->UpdateRotateByMouse();

	Owner->LerpToFocusOnPlayerThrowCameraLocation();

	if (!Owner->HasStateFlag(TPSCamera::StateFlg::PlayerThrowFocus))
	{
		m_stateMachine->ChangeState<TPSCameraFollowPlayerState>();
	}
}