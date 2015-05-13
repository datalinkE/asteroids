#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"

using namespace glm;

GameObject::GameObject(GameEngine* gameEngine, vec3 position, vec4 color, float boundingRadius, float timeToLive, vec3 velocity)
    : mDeleted(false)
    , mInterfered(false)
    , mTimeToLive(timeToLive)
    , mTime(0.0f)
    , mPosition(position)
    , mVelocity(velocity)
    , mBoundingRadius(boundingRadius)
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
    mModelMatrix = translate( mPosition);

    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, mColor, GL_TRIANGLE_FAN);
}

void GameObject::impulse(const vec3& delta)
{
    mVelocity += delta;
}
