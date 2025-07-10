#include "PlayerIdleState.h"
#include "../Move/PlayerMoveState.h"
#include "../StickyBombThrow/PlayerPrepareStickyBombThrowState.h"

#include "../../../../GameObject/Actor/Player/Player.h"
#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../../../../Scene/BaseScene/BaseScene.h"
#include "../../../../Scene/SceneManager.h"

void PlayerIdleState::Enter(Player* Owner)
{
	// SHOULD DELETE BEFORE RELEASE
	KdDebugGUI::Instance().AddLog("Player State : Idle\n");

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