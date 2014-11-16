#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"

using namespace glm;

GameObject::GameObject(GameEngine* gameEngine, vec3 position, float boundingRadius, float timeToLive, vec3 velocity)
    : mDeleted(false)
    , mTimeToLive(timeToLive)
    , mTime(0.0f)
    , mPosition(position)
    , mVelocity(velocity)
    , mBoundingRadius(boundingRadius)
    , mEngine(gameEngine)
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
}

void GameObject::interfere()
{
    if (mTimeToLive > 0 && mTime > mTimeToLive)
    {
        mDeleted = true;
    }
}

void GameObject::draw()
{
    mModelMatrix = translate(mPosition);
    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, vec4(0.0f, 0.0f, 1.0f, 1.0f), GL_TRIANGLE_FAN);
}


