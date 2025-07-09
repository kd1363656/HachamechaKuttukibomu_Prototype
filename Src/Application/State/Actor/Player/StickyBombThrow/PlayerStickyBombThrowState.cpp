#include "PlayerStickyBombThrowState.h"

#include "../../../../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../../../../Scene/SceneManager.h"
#include "../../../../Scene/BaseScene/BaseScene.h"

void PlayerStickyBombThrowState::Enter(Player* Owner)
{
	KdDebugGUI::Instance().AddLog("Player State : StickyBombThrowState\n");

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
void PlayerStickyBombThrowState::Update(Player* Owner)
{

}