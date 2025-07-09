#pragma once
#include "../CameraBase.h"

#include "../../../State/StateMachine.h"

class TPSCamera : public CameraBase
{
public:

	enum class StateFlg
	{
		FollowPlayer     = 1 << 0, 
		PlayerThrowFocus = 1 << 1,
	};

	TPSCamera()							{}
	~TPSCamera()			override	{}

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<TPSCamera>(); }
	
	void Init() override;

	void Update    () override;
	void PostUpdate() override;

	void LerpToFocusOnPlayerThrowCameraLocation();
	void LerpToStandardCameraLocation          ();

	bool HasStateFlag(StateFlg Flag);

	void EnableStateFlag (StateFlg Flag);
	void DisableStateFlag(StateFlg Flag);

private:

	void AdjustLocation();

	const Math::Vector3 STANDARD_CAMERA_LOCATION              = { 0.0f  , 1.5f  , -3.0f  };
	const Math::Vector3 FOCUS_ON_PLAYER_THROW_CAMERA_LOCATION = { 0.50f , 1.60f , -1.00f };
	
	const float LERP_THREDS_HOLD = 0.005f;

	StateMachine<TPSCamera> m_stateMachine;

	// "フラグ"でメモリを圧迫したくなく、たくさんの状態を管理したい、そのためのビットフラグ
	uint32_t m_stateChangeFlg = 0u;
};