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

void Player::onPadMove(vec3 direction, vec3 delta, float timeDelta)
{
    float value = std::min(length(direction), 0.5f);
    if (value > std::numeric_limits<float>::epsilon())
    {
        mRotation = direction;
        if (value == 0.5f)
        {
            impulse(direction*0.03f);
        }
    }
    else
    {
        DLOG() << "deceleration";
        impulse(-mVelocity * 0.5f);
    }
}

void Player::impulse(const vec3& delta)
{
    vec3 oldVelocity = mVelocity;
    GameObject::impulse(delta);
    if (length(mVelocity) > 2.0f)
    {
        mVelocity = oldVelocity;
    }
}
