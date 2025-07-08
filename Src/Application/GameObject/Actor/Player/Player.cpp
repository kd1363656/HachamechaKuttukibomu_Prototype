#include "Player.h"

#include "../../../State/Actor/Player/Idle/PlayerIdleState.h"

#include "../../../Utility/InputManager/RawInputManager.h"

#include "../../../main.h"

#include "../../../Scene/SceneManager.h"
#include "../../../Scene/BaseScene/BaseScene.h"

#include "../../Camera/CameraBase.h"

#include "../../../Utility/CommonConstant.h"

void Player::Init()
{
	ActorBase::Init();

	m_transform.scale = CommonConstant::STANDARD_SCALE;

	m_stateMachine.Start(this);
	m_stateMachine.ChangeState<PlayerIdleState>();
}

void Player::PostLoadInit()
{
	ActorBase::PostLoadInit();

	AssignCameraFromScene();
}

void Player::Update()
{
	m_moveDirection = Math::Vector3::Zero;
	m_movement      = Math::Vector3::Zero;

	UpdateGravity();

	m_stateMachine.Update();

	AdjustFacingDirectionToCamera();
}

void Player::Move()
{
	if (IsInvalidMoveKeyPressed()) { return; }

	const float deltaTime_ = Application::Instance().GetScaledDeltaTime();

	// キー入力を確認してベクトルを計算
	AddMoveDirectionIfKeyPressed('D' , m_moveDirection , Math::Vector3::Right   );
	AddMoveDirectionIfKeyPressed('A' , m_moveDirection , Math::Vector3::Left    );
	AddMoveDirectionIfKeyPressed('W' , m_moveDirection , Math::Vector3::Backward);
	AddMoveDirectionIfKeyPressed('S' , m_moveDirection , Math::Vector3::Forward );

	Math::Vector3 moveDirection_ = m_moveDirection;

	moveDirection_.Normalize();
	
	Math::Matrix  cameraRotationMat_  = Math::Matrix::Identity;
	Math::Vector3 finalMoveDirection_ = Math::Vector3::Zero;

	if(auto wp_ = m_camera.lock())
	{
		cameraRotationMat_ = wp_->GetRotationYMatrix();
	}

	// カメラの向いている向きからどの方向に動くかを決める
	finalMoveDirection_ = Math::Vector3::Transform(moveDirection_ , cameraRotationMat_);
	
	m_movement +=  m_maxMoveSpeed * finalMoveDirection_ * deltaTime_;
	m_movement.y = 0.0f;
}

void Player::PrepareStickyBombThrow()
{

}

void Player::AdjustFacingDirectionToCamera()
{
	if (m_moveDirection.LengthSquared() < CommonConstant::MOVE_DIRECTION_EPSILON) { return; }

	Math::Vector3 targetDirection_ = Math::Vector3::Zero;

	Math::Matrix cameraRotateY_ = Math::Matrix::Identity;

	if (auto wp_ = m_camera.lock())
	{
		cameraRotateY_ = wp_->GetRotationYMatrix();
	}

	// 進行方向がキャラクターの目的となる向きの角度
	targetDirection_ = m_moveDirection.TransformNormal(m_moveDirection , cameraRotateY_);

	// 現在の角度を行列変換する
	Math::Matrix nowRotationMatY_ = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_transform.rotation.y));

	// よくわからんので復習
	// プレイヤーのモデルが"Rotation.y"が"0.0f"の時に向いている方向をもとに考える
	Math::Vector3 nowDirection_ = Math::Vector3::TransformNormal(-Math::Vector3::UnitZ , nowRotationMatY_);

	// 双方、内積を求めるために正規化
	targetDirection_.Normalize();
	nowDirection_.Normalize   ();

	// 内積を算出
	float dot_ = nowDirection_.Dot(targetDirection_);

	dot_ = std::clamp(dot_, CommonConstant::DOT_PRODUCT_MIN , CommonConstant::DOT_PRODUCT_MAX);
	// 算出したなす角をアークコサイン化すると角度になる
	float angleY_ = DirectX::XMConvertToDegrees(acos(dot_));

	// TODO
	// 加減速があった方がエネルギッシュ
	if(angleY_ >= 0.1f)
	{
		if(angleY_ > 5.0f)
		{
			angleY_ = 5.0f;
		}
	}

	// 外積で回転軸を求める
	Math::Vector3 cross_ = nowDirection_.Cross(targetDirection_);

	if(cross_.y >= 0.0f)
	{
		m_transform.rotation.y += angleY_;
		
		if(m_transform.rotation.y > CommonConstant::ALL_DEGREE)
		{
			m_transform.rotation.y -= CommonConstant::ALL_DEGREE;
		}
	}
	else
	{
		m_transform.rotation.y -= angleY_;

		if (m_transform.rotation.y < 0.0f)
		{
			m_transform.rotation.y += CommonConstant::ALL_DEGREE;
		}
	}
}

// 進行方向と反対のキーが同時に押されていたら移動方向がなくなるので実行しない
bool Player::IsInvalidMoveKeyPressed()
{
	auto& input_ = RawInputManager::GetInstance();

	bool isPressRightLeftKey_ = input_.GetKeyState('D') && input_.GetKeyState('A');
	bool isPressUpDownKey_    = input_.GetKeyState('W') && input_.GetKeyState('S');

	if(isPressRightLeftKey_ ||
	   isPressUpDownKey_)
	{
		return true;
	}

	return false;
}

bool Player::IsMoveKeyPressed()
{
	auto& input_ = RawInputManager::GetInstance();

	if (IsInvalidMoveKeyPressed()) 
	{ 
		return false; 
	}

	bool isPressRightOrLeftKey_ = input_.GetKeyState('D') || input_.GetKeyState('A');
	bool isPressUpOrDownKey_    = input_.GetKeyState('W') || input_.GetKeyState('S');

	if(isPressRightOrLeftKey_ || isPressUpOrDownKey_)
	{
		return true;
	}
	return false;
}

bool Player::IsStickyBombThrowKeyPressed()
{
	auto& input_ = RawInputManager::GetInstance();

	if(input_.GetMouseData().isClickLeft)
	{
		return true;
	}

	return false;
}

void Player::AddMoveDirectionIfKeyPressed(int VirtualKeyCode, Math::Vector3& MoveDirection, const Math::Vector3& WantAddDirection)
{
	auto& input_ = RawInputManager::GetInstance();

	if (IsInvalidMoveKeyPressed()) { return; }

	if (input_.GetKeyState(VirtualKeyCode))
	{
		MoveDirection += WantAddDirection;
	}
}

void Player::AssignCameraFromScene()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	// もしシーンが存在しなければアクセスできないので"return"
	if (!currentScene_) { return; }

	// カメラのポインタをプレイヤーの"std::weak_ptr"に格納
	for (auto& cache_ : currentScene_->GetCacheObjectList<CameraBase>())
	{
		m_camera = cache_;
	}
}

void Player::FixMatrix()
{
	m_transform.location += m_movement;

	ActorBase::FixMatrix();
}