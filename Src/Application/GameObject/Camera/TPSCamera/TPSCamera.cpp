#include "TPSCamera.h"

#include "../../Actor/Player/Player.h"

#include "../../../State/Camera/TPSCamera/FollowPlayer/TPSCameraFollowPlayerState.h"

#include "../../../main.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 注視点
	m_location  = STANDARD_CAMERA_LOCATION;
	m_mLocalPos = Math::Matrix::CreateTranslation(m_location);

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	m_stateMachine.Start(this);
	m_stateMachine.ChangeState<TPSCameraFollowPlayerState>();
}

void TPSCamera::Update()
{
	CameraBase::Update();

	m_stateMachine.Update();
}

void TPSCamera::PostUpdate()
{
	AdjustLocation();

	CameraBase::PostUpdate();
}

void TPSCamera::LerpToFocusOnPlayerThrowCameraLocation()
{
	const float deltaTime_ = Application::Instance().GetScaledDeltaTime();

	m_location = Math::Vector3::Lerp(m_location, FOCUS_ON_PLAYER_THROW_CAMERA_LOCATION, 3.0f * deltaTime_);
}
void TPSCamera::LerpToStandardCameraLocation()
{
	const float deltaTime_ = Application::Instance().GetScaledDeltaTime();

	m_location = Math::Vector3::Lerp(m_location, STANDARD_CAMERA_LOCATION, 3.0f * deltaTime_);
}

bool TPSCamera::HasStateFlag(StateFlg Flag)
{
	const uint32_t flg_ = static_cast<uint32_t>(Flag);

	return (m_stateChangeFlg & flg_) != 0;
}
void TPSCamera::EnableStateFlag(StateFlg Flag)
{
	const uint32_t flg_ = static_cast<uint32_t>(Flag);

	m_stateChangeFlg |= flg_;
}
void TPSCamera::DisableStateFlag(StateFlg Flag)
{
	const uint32_t flg_ = static_cast<uint32_t>(Flag);

	m_stateChangeFlg &= ~flg_;
}

void TPSCamera::AdjustLocation()
{
	auto _spTarget = m_targetPlayer.lock();

	if (!_spTarget) { return; }

	// ↓めり込み防止の為の座標補正計算↓
	// ①当たり判定(レイ判定)用の情報作成
	KdCollider::RayInfo rayInfo;
	// レイの発射位置を設定
	rayInfo.m_pos = GetPos();

	// レイの発射方向を設定
	rayInfo.m_dir = Math::Vector3::Down;
	// レイの長さを設定
	rayInfo.m_range = 1000.f;
	if (_spTarget)
	{
		Math::Vector3 _targetPos = _spTarget->GetPos();
		_targetPos.y += 0.1f;
		rayInfo.m_dir = _targetPos - GetPos();
		rayInfo.m_range = rayInfo.m_dir.Length();
		rayInfo.m_dir.Normalize();
	}

	// 当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject> wpGameObj : m_wpHitObjectList)
	{
		std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
		if (spGameObj)
		{
			std::list<KdCollider::CollisionResult> retRayList;
			spGameObj->Intersects(rayInfo, &retRayList);

			// ③ 結果を使って座標を補完する
			// レイに当たったリストから一番近いオブジェクトを検出
			float maxOverLap = 0;
			Math::Vector3 hitPos = {};
			bool hit = false;
			for (auto& ret : retRayList)
			{
				// レイを遮断しオーバーした長さが
				// 一番長いものを探す
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hit = true;
				}
			}
			if (hit)
			{
				// 何かしらの障害物に当たっている
				Math::Vector3 _hitPos = hitPos;
				_hitPos += rayInfo.m_dir * 0.4f;
				SetPos(_hitPos);
			}
		}
	}
}