#include "Player.h"
#include "Logger.hpp"
#include <glm/gtx/projection.hpp>

using namespace glm;

Player::Player(GameEngine* engine)
	: GameObject(engine, vec3(0.0f), vec4(0.2f, 0.4f, 0.4f, 1.0f), 1.0f)
{
	DLOG();
}

Player::~Player()
{
	DLOG();
}

void Player::onImpact(GameObject* other)
{
	DLOG();
	vec3 centerLine = position() - other->position();
	vec3 delta = -2.0f * proj(mVelocity, centerLine);
    mVelocity += delta;
    mPosition += 0.05f * mVelocity;
}

