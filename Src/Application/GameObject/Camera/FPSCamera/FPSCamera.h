#pragma once
#include "../CameraBase.h"

class FPSCamera : public CameraBase
{
public:
	FPSCamera()							{}
	~FPSCamera()			override	{}

	uint32_t GetTypeID()const override { return GameObjectID::GetTypeID<FPSCamera>(); }

	void Init()				override;
	void PostUpdate()		override;
};