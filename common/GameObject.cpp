#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"
#include "Rotation.h"

using namespace glm;

const vec3 GameObject::sZeroRotation = vec3(1.0f, 0.0f, 0.0f);

GameObject::GameObject(GameEngine* gameEngine, vec3 position, vec4 color, float size, float timeToLive, vec3 velocity)
    : mDeleted(false)
    , mInterfered(false)
    , mTimeToLive(timeToLive)
    , mTime(0.0f)
    , mPosition(position)
    , mVelocity(velocity)
    , mRotation(sZeroRotation)
    , mSize(size)
    , mEngine(gameEngine)
    , mColor(color)
{
    DLOG();
}

GameObject::~GameObject()
{
    DLOG();
}

void GameObject::onTick(float timeDelta)
{
    mTime += timeDelta;
    if (mTimeToLive > 0 && mTime > mTimeToLive)
    {
        mDeleted = true;
    }

    mPosition += timeDelta * mVelocity;
}

void GameObject::setPosition(vec3 position)
{
    mPosition = position;
}

void GameObject::setVelocity(vec3 velocity)
{
    mVelocity = velocity;
}

void GameObject::doImpacts()
{
    for (GameObject* other : mEngine->mCollidables->retrieve(this))
    {
        if (other != this)
        {
            float dist = distance(other->position(), this->position());
            float rr = other->boundingRadius() + this->boundingRadius();
            if (dist < rr)
            {
                other->onImpact(this);
                onImpact(other);
            }
        }
    }
    mInterfered = true;
}

void GameObject::onImpact(GameObject* other)
{
    this->mColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);
}

void GameObject::draw(GLuint drawMode)
{
    mModelMatrix = translate(mPosition) * rotationBetweenVectors(sZeroRotation, mRotation);
    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, mSize, mColor, drawMode);
}

void GameObject::impulse(const vec3& delta)
{
    mVelocity += delta;
}
