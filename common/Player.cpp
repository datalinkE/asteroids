#include "Player.h"
#include "Logger.hpp"
#include "Geometry.hpp"
#include "GameEngine.h"
#include <glm/gtx/projection.hpp>

using namespace glm;

const float Player::MAX_PROJECTILE_POWER = 0.3f;

Player::Player(GameEngine* engine)
	: GameObject(engine, vec3(0.0f), vec4(0.2f, 0.4f, 0.4f, 1.0f), 1.0f)
    , mProjectilePower(MAX_PROJECTILE_POWER)
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

void Player::onPush(float timeDelta)
{
    if (mProjectilePower >= MAX_PROJECTILE_POWER)
    {
        //shoot projectile
        mProjectilePower = 0.0f;
        vec3 rot = normalize(mRotation);
        mEngine->mObjects.insert(mEngine->mObjects.end(),
                                GameObjectPtr(new GameObject(mEngine,
                                                             mPosition + rot,
                                                             vec4(0.7f, 0.0f, 0.7f, 3.0f),
                                                             0.5f,
                                                             10.0f,
                                                             rot * 10.0f)));
    }
    else
    {
        mProjectilePower += timeDelta;
    }
}
