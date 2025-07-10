#include "PlayerMoveState.h"
#include "../Idle/PlayerIdleState.h"
#include "../StickyBombThrow/PlayerPrepareStickyBombThrowState.h"

#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../../../GameObject/Actor/Player/Player.h"

#include "../../../../Scene/SceneManager.h"
#include "../../../../Scene/BaseScene/BaseScene.h"

void PlayerMoveState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : Move\n");

	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();
	
	if (!scene_) { return; }
	
	for (auto& cache_ : scene_->GetCacheObjectList<TPSCamera>())
	{
		if (auto wp_ = cache_.lock())
		{
			// 注意
			// ここでカメラのステートを変更する処理を書いている
			wp_->DisableStateFlag(TPSCamera::StateFlg::PlayerThrowFocus);
		}
	}
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