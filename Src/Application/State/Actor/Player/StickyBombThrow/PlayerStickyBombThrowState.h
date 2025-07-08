#pragma once
#include "../../../StateBase.h"

class Player;

class PlayerStickyBombThrowState : public StateBase<Player>
{
public:

	PlayerStickyBombThrowState () = default;
	~PlayerStickyBombThrowState() = default;

	void Enter (Player* Owner)override;
	void Update(Player* Owner)override;
};