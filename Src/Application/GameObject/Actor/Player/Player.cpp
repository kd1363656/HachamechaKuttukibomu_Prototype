#include "Player.h"

#include "../../../State/Actor/Player/Idle/PlayerIdleState.h"

#include "../../../Utility/InputManager/RawInputManager.h"

#include "../../../main.h"

#include "../../../Scene/SceneManager.h"
#include "../../../Scene/BaseScene/BaseScene.h"

#include "../../Camera/CameraBase.h"

void Player::Init()
{
	ActorBase::Init();

	m_materialInfo.assetFilePath = "Asset/Models/Player/MF_IDLE/MF_IDLE.gltf";

	if(!m_materialInfo.modelWork) 
	{ 
		m_materialInfo.modelWork = std::make_shared<KdModelWork>();			  
	}
	if (m_materialInfo.modelWork) 
	{
		m_materialInfo.modelWork->SetModelData(m_materialInfo.assetFilePath);
	}

	// TODO
	m_transform.scale = { 1.0f , 1.0f , 1.0f };

	m_stateMachine.Start(this);
	m_stateMachine.ChangeState<PlayerIdleState>();
}

void Player::PostLoadInit()
{
	auto currentScene_ = SceneManager::GetInstance().GetCurrentScene().lock();

	// もしシーンが存在しなければアクセスできないので"return"
	if(!currentScene_)
	{
		return;
	}

	// カメラのポインタをプレイヤーの"std::weak_ptr"に格納
	for(auto& cache_ : currentScene_->GetCacheObjectList<CameraBase>())
	{
		m_camera = cache_.lock();
	}
}

void Player::Update()
{
	m_moveDirection = Math::Vector3::Zero;
	m_movement      = Math::Vector3::Zero;

	m_stateMachine.Update();
}

void Player::Move()
{
	if (IsInvalidMoveKeyPressed())
	{
		return;
	}

	const float deltaTime_ = Application::Instance().GetScaledDeltaTime();

	// キー入力を確認してベクトルを計算
	AddMoveDirectionIfKeyPressed(VK_RIGHT ,  Math::Vector3::UnitX);
	AddMoveDirectionIfKeyPressed(VK_LEFT  , -Math::Vector3::UnitX);
	AddMoveDirectionIfKeyPressed(VK_UP	  ,  Math::Vector3::UnitZ);
	AddMoveDirectionIfKeyPressed(VK_DOWN	  , -Math::Vector3::UnitZ);

	m_moveDirection.Normalize();

	Math::Matrix  cameraRotationMat_  = Math::Matrix::Identity;
	Math::Vector3 finalMoveDirection_ = Math::Vector3::Zero;

	if(auto wp_ = m_camera.lock())
	{
		cameraRotationMat_ = wp_->GetRotationMatrix();
	}

	finalMoveDirection_ = Math::Vector3::Transform(m_moveDirection , cameraRotationMat_);
	
	m_movement +=  m_maxMoveSpeed * finalMoveDirection_ * deltaTime_;
	m_movement.y = 0.0f;
}

// 進行方向と反対のキーが同時に押されていたら移動方向がなくなるので実行しない
bool Player::IsInvalidMoveKeyPressed()
{
	auto& input_ = RawInputManager::GetInstance();

	bool isPressRightLeftKey_ = input_.GetKeyState(VK_RIGHT) && input_.GetKeyState(VK_LEFT);
	bool isPressUpDownKey_    = input_.GetKeyState(VK_UP   ) && input_.GetKeyState(VK_DOWN);

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

	bool isPressRightOrLeftKey_ = input_.GetKeyState(VK_RIGHT) || input_.GetKeyState(VK_LEFT);
	bool isPressUpOrDownKey_    = input_.GetKeyState(VK_UP   ) || input_.GetKeyState(VK_DOWN);

	if(isPressRightOrLeftKey_ || isPressUpOrDownKey_)
	{
		return true;
	}
	return false;
}


void Player::AddMoveDirectionIfKeyPressed(int VirtualKeyCode, Math::Vector3 MoveDirection)
{
	auto& input_ = RawInputManager::GetInstance();

	if (IsInvalidMoveKeyPressed()) 
	{ 
		return;
	}

	if (input_.GetKeyState(VirtualKeyCode)) 
	{ 
		m_moveDirection += MoveDirection;
	}
}

void Player::FixMatrix()
{
	m_transform.location += m_movement;

	ActorBase::FixMatrix();
}