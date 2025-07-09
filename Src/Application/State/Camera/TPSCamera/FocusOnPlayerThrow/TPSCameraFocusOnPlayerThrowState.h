#pragma once
#include "../../../StateBase.h"

class TPSCamera;

class TPSCameraFocusOnPlayerThrowState : public StateBase<TPSCamera>
{
public:

	TPSCameraFocusOnPlayerThrowState () = default;
	~TPSCameraFocusOnPlayerThrowState() = default;

	void Update(TPSCamera* Owner) override;
};