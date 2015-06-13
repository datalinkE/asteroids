#include "Player.h"
#include "Logger.hpp"
#include "Geometry.hpp"
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

    if (value > Geometry::eps)
    {
        mRotation = direction;
        if (value == 0.5f)
        {
            impulse(direction*0.08f);
        }
    }
    else
    {
        impulse(-mVelocity * 0.003f);
    }
}

void Player::impulse(const vec3& delta)
{
    const float MAX_VELOCITY = 2.5f;
    GameObject::impulse(delta);
    if (length(mVelocity) > MAX_VELOCITY)
    {
        mVelocity = glm::normalize(mVelocity) * MAX_VELOCITY;
    }
}
