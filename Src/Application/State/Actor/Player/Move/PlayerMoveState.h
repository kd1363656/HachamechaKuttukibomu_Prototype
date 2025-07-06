#pragma once
#include "../../../StateBase.h"

class Player;

class PlayerMoveState : public StateBase<Player>
{
public:

	PlayerMoveState () = default;
	~PlayerMoveState() = default;

	void Enter (Player* Owner)override;
	void Update(Player* Owner)override;
	void Exit  (Player* Owner)override;
};