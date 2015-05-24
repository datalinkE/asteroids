#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"
#include "Rotation.h"

using namespace glm;

GameObject::GameObject(GameEngine* gameEngine, vec3 position, vec4 color, float size, float timeToLive, vec3 velocity)
    : mDeleted(false)
    , mInterfered(false)
    , mTimeToLive(timeToLive)
    , mTime(0.0f)
    , mPosition(position)
    , mVelocity(velocity)
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

void GameObject::move(float timeDelta)
{
    mTime += timeDelta;
    mPosition += timeDelta * mVelocity;
    //mInterfered = false;
}

void GameObject::doImpacts()
{
    if (mTimeToLive > 0 && mTime > mTimeToLive)
    {
        mDeleted = true;
    }
    else
    {
        for (GameObject* other : mEngine->mCollidables->retrieve(this))
        {
            if (other != this)
            {
                float dist = distance(other->position(), this->position());
                float rr = other->boundingRadius() + this->boundingRadius();
                if (dist < rr)
                {
                	//DLOG() << "impact" << ARG(dist) << ARG(rr);;
                	other->onImpact(this);
                	onImpact(other);
                }
            }
        }
    }

    mInterfered = true;
}


void GameObject::onImpact(GameObject* other)
{
    this->mColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);
    //DLOG() << mPosition[0] << mPosition[1];
}

void GameObject::draw()
{
    const float velocityModule = length(mVelocity);
    const vec3 zeroRotation = vec3(1.0f, 0.0f, 0.0f);

    mModelMatrix = translate(mPosition) * rotationBetweenVectors(zeroRotation, velocityModule > 0.0f ? mVelocity : zeroRotation);

    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, mSize, mColor, GL_LINE_STRIP);
}

void GameObject::impulse(const vec3& delta)
{
    mVelocity += delta;
}
