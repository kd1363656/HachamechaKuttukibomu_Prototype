#pragma once
#include "../../../StateBase.h"

class TPSCamera;

class TPSCameraFollowPlayerState : public StateBase<TPSCamera>
{
public:

	TPSCameraFollowPlayerState () = default;
	~TPSCameraFollowPlayerState() = default;

	void Update(TPSCamera* Owner) override;
};