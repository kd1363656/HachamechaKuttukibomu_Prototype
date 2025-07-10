#include "PlayerPrepareStickyBombThrowState.h"
#include "PlayerStickyBombThrowState.h"
#include "../Idle/PlayerIdleState.h"
#include "../Move/PlayerMoveState.h"

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

	Owner->Move();
	Owner->PrepareStickyBombThrow();

	// TODO
	if (isPrepareStickyBombThrow_ && !ImGui::GetIO().WantCaptureMouse)
	{
		m_stateMachine->ChangeState<PlayerStickyBombThrowState>();
	}

	// ボム投げをやめる
	if (isCancelStickyBombThrowKeyPressed_)
	{
		// 動いているキーが押されていたら動いているステートに移行
		// じゃないと"Idle"から"Move"に移行するのにステートを生成していて無駄
		const bool isMove = Owner->IsMoveKeyPressed();
		if(isMove)
		{
			m_stateMachine->ChangeState<PlayerMoveState>();
		}
		else
		{
			m_stateMachine->ChangeState<PlayerIdleState>();
		}
	}
}