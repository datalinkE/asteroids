#pragma once

#include "GameObject.h"
#include "DirectionPad.h"
#include "Button.h"

class Player: public GameObject, public DirectionPadListener, public ButtonListener {
public:
	Player(GameEngine* engine);
	virtual ~Player();

	virtual void impulse(const glm::vec3& delta);
	virtual void onImpact(GameObject* other);
	virtual void onPadMove(glm::vec3 direction, glm::vec3 delta, float timeDelta);
	virtual void onPush(float timeDelta);

private:
	float mProjectilePower;
    const static float MAX_PROJECTILE_POWER;
};
