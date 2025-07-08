#include "PlayerPrepareStickyBombThrowState.h"

#include "../../../../GameObject/Actor/Player/Player.h"

void PlayerPrepareStickyBombThrowState::Enter(Player* Owner)
{

}

void PlayerPrepareStickyBombThrowState::Update(Player* Owner)
{
	Owner->PrepareStickyBombThrow();
}