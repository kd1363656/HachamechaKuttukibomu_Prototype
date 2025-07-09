#include "PlayerPrepareStickyBombThrowState.h"
#include "PlayerStickyBombThrowState.h"
#include "../Idle/PlayerIdleState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../../../../Scene/SceneManager.h"
#include "../../../../Scene/BaseScene/BaseScene.h"

void PlayerPrepareStickyBombThrowState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : PrepareStickyBombThrowState\n");

	auto scene_ = SceneManager::GetInstance().GetCurrentScene().lock();
	
	if (!scene_) { return; }
	
	for (auto& cache_ : scene_->GetCacheObjectList<TPSCamera>())
	{
		if (auto wp_ = cache_.lock())
		{
			// 注意
			// ここでカメラのステートを変更する処理を書いている
			wp_->EnableStateFlag(TPSCamera::StateFlg::PlayerThrowFocus);
		}
	}
}

void PlayerPrepareStickyBombThrowState::Update(Player* Owner)
{
	const bool isPrepareStickyBombThrow_          = Owner->IsStickyBombThrowKeyPressed      ();
	const bool isCancelStickyBombThrowKeyPressed_ = Owner->IsCancelStickyBombThrowKeyPressed();

	Owner->PrepareStickyBombThrow();

	// TODO
	if (isPrepareStickyBombThrow_ && !ImGui::GetIO().WantCaptureMouse)
	{
		m_stateMachine->ChangeState<PlayerStickyBombThrowState>();
	}

	// ボム投げをやめる
	if (isCancelStickyBombThrowKeyPressed_)
	{
		m_stateMachine->ChangeState<PlayerIdleState>();
	}
}