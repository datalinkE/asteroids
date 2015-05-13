#pragma once

#include "GameObject.h"

class Player: public GameObject {
public:
	Player(GameEngine* engine);
	virtual ~Player();

	virtual void onImpact(GameObject* other);
};
