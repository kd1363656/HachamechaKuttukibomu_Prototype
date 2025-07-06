#pragma once
#include "../CameraBase.h"

class Player;

class DebugCamera : public CameraBase
{
public:

	DebugCamera ()         = default;
	~DebugCamera()override = default;

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<DebugCamera>(); }

	void Init()	override;
	
	void PostUpdate() override;

private:

	void Move  ();
	void Rotate();

	const Math::Vector3 FIRST_LOCATION = {0.0f , 0.0f , -10.0f};

	const float NORMAL_MOVE_SPEED = 0.3f;

	bool m_shouldLockPreviousLocation = false;;
};