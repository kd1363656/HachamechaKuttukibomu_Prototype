#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera()							{}
	~TPSCamera()			override	{}

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<TPSCamera>(); }
	
	void Init()				override;
	void PostUpdate()		override;

private:

	Math::Vector3 POINT_OF_FIXATION                          = { 0.0f  , 1.5f  ,  -3.0f };
	Math::Vector3 POINT_OF_PREPARE_STICK_BOMB_THROW_LOCATION = { 0.50f , 1.60f , -0.90f };
	Math::Vector3 POINT_OF_PREPARE_STICK_BOMB_THROW_ROTATION = { 0.50f , 66.0f , -0.90f };
};