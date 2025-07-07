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

	Math::Vector3 POINT_OF_FIXATION = { 0.0f , 1.5f , -3.0f };
};