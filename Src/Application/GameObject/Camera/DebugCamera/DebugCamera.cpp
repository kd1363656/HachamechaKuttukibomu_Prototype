#include "DebugCamera.h"

#include "../../../Utility/InputManager/RawInputManager.h"

#include "../../../Scene/SceneManager.h"

#include "../../../Scene/BaseScene/BaseScene.h"

void DebugCamera::Init()
{
	CameraBase::Init();

	m_location = FIRST_LOCATION;

	m_shouldLockPreviousLocation = false;
}

void DebugCamera::PostUpdate()
{
	auto& input_ = RawInputManager::GetInstance();

	Move  ();
	Rotate();

	// 行列の確定
	m_mLocalPos = Math::Matrix::CreateTranslation(m_location);
	m_mRotation = GetRotationMatrix();

	m_mWorld = m_mLocalPos * m_mRotation;
}

void DebugCamera::Move()
{
	auto& input_ = RawInputManager::GetInstance();

	Math::Vector3 movement_  = Math::Vector3::Zero;
	Math::Vector3 dir_       = Math::Vector3::Zero;

	// "XY"か"XZ"かをクリックしているボタンによって変える
	// "XZ"
	if (input_.GetMouseData().isClickLeft)
	{
		if (input_.GetKeyState('A')) { dir_.x -= Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('D')) { dir_.x += Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('W')) { dir_.z += Math::Vector3::UnitZ.z; }
		if (input_.GetKeyState('S')) { dir_.z -= Math::Vector3::UnitZ.z; }
	}
	// "YZ"
	else if(input_.GetMouseData().isClickMiddle)
	{
		if (input_.GetKeyState('A')) { dir_.x -= Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('D')) { dir_.x += Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('W')) { dir_.y += Math::Vector3::UnitY.y; }
		if (input_.GetKeyState('S')) { dir_.y -= Math::Vector3::UnitY.y; }
	}

	dir_.Normalize();

	// "m_mRotation"の3方向のベクトルを取得して正規化した角度と移動速度を掛け合わせる
	// カメラの回転に応じたベクトルを取得できる
	// "Transform"を使う場合回転以外の行列を合成していないほうが複雑でなく安全
	movement_ = Math::Vector3::Transform(dir_ , m_mRotation)  * NORMAL_MOVE_SPEED;

	m_location += movement_;
}

void DebugCamera::Rotate()
{
	auto& input_ = RawInputManager::GetInstance();

	// 回転角調整モード中はマウスの座標が動かないようにする
	if (input_.GetMouseData().isClickRight)
	{
		// 右クリックをし始めたら一回だけ今の座標を記録
		if (!m_shouldLockPreviousLocation)
		{
			m_shouldLockPreviousLocation = true;
		}

		UpdateRotateByMouse();
	}
	else
	{
		m_shouldLockPreviousLocation = false;
	}
}