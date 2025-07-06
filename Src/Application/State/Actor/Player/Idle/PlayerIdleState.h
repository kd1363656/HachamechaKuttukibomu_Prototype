#pragma once
#include "../../../StateBase.h"

class Player;

class PlayerIdleState : public StateBase<Player>
{
public:

	PlayerIdleState () = default;
	~PlayerIdleState() = default;

	void Enter (Player* Owner)override;
	void Update(Player* Owner)override;
	void Exit  (Player* Owner)override;
};