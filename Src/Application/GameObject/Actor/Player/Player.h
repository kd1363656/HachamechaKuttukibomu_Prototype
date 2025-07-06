#pragma once
#include "../ActorBase.h"

#include "../../../State/StateMachine.h"

class CameraBase;

class Player : public ActorBase
{
public:

	Player ()         = default;
	~Player()override = default;

	uint32_t GetTypeID() const override{ return GameObjectID::GetTypeID<Player>(); }

	void Init        () override;
	void PostLoadInit() override;

	void Update()override;

	void Move();

	bool IsInvalidMoveKeyPressed();
	bool IsMoveKeyPressed       ();

private:

	StateMachine<Player> m_stateMachine;

	std::weak_ptr<CameraBase> m_camera;

	void AddMoveDirectionIfKeyPressed(int VirtualKeyCode , Math::Vector3 MoveDirection);

protected:

	void FixMatrix();
};