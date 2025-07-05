#include "DebugCamera.h"

#include "../../../Utility/InputManager/RawInputManager.h"

void DebugCamera::Init()
{
	CameraBase::Init();

	m_location = FIRST_LOCATION;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void DebugCamera::PostUpdate()
{
	auto& input_ = RawInputManager::GetInstance();

	Move();

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

	if (input_.GetMouseData().isClickLeft)
	{
		if (input_.GetKeyState('A')) { dir_.x -= Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('D')) { dir_.x += Math::Vector3::UnitX.x; }
		if (input_.GetKeyState('W')) { dir_.z += Math::Vector3::UnitZ.z; }
		if (input_.GetKeyState('S')) { dir_.z -= Math::Vector3::UnitZ.z; }
	}

	dir_.Normalize();

	// 正規化した角度と移動速度を掛け合わせる
	movement_ = dir_ * NORMAL_MOVE_SPEED;

	m_location += movement_;
}
