#pragma once
#include "../ActorBase.h"

class Player : public ActorBase
{
public:

	Player ()         = default;
	~Player()override = default;

	REGISTER_CLASS_NAME(Player);

	uint32_t GetTypeID()const override{ return GameObjectID::GetTypeID<Player>(); }

	void Init()override;
};