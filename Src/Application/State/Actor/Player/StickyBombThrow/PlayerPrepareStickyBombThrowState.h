#pragma once
#include "../../../StateBase.h"

class Player;

class PlayerPrepareStickyBombThrowState : public StateBase<Player>
{
public:

	PlayerPrepareStickyBombThrowState () = default;
	~PlayerPrepareStickyBombThrowState() = default;

	void Enter (Player* Owner)override;
	void Update(Player* Owner)override;

private:

	bool m_isThrowStickyBombKeyReleased = false;
};