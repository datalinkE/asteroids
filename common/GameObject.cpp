#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"

using namespace glm;

GameObject::GameObject(GameEngine* gameEngine, vec3 position, float boundingRadius, float timeToLive, vec3 velocity)
    : mDeleted(false)
    , mInterfered(false)
    , mTimeToLive(timeToLive)
    , mTime(0.0f)
    , mPosition(position)
    , mVelocity(velocity)
    , mBoundingRadius(boundingRadius)
    , mEngine(gameEngine)
    , mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
    DLOG();
}

GameObject::~GameObject()
{
    DLOG();
}

void GameObject::move(float timeDelta)
{
    mTime += timeDelta;
    mPosition += timeDelta * mVelocity;
    mInterfered = false;
}

void GameObject::interfere()
{
    if (mTimeToLive > 0 && mTime > mTimeToLive)
    {
        mDeleted = true;
    }
    else
    {
        for (GameObject* other : mEngine->mCollidables->retrieve(this))
        {
            if (other != this && !other->interferedThisTick())
            {
                float dist = distance(other->mPosition, this->mPosition);
                float rr = other->mBoundingRadius + this->mBoundingRadius;
                if (dist < rr)
                {
                    other->mColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                    this->mColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }
    }
    mInterfered = true;
}

void GameObject::draw()
{
    mModelMatrix = translate( mPosition);

    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, mColor, GL_TRIANGLE_FAN);
}


