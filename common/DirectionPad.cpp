#include "DirectionPad.h"
#include "Logger.hpp"

using namespace glm;

const vec4 PAD_COLOR(0.5f, 0.3f, 0.0f, 1.0f);
const vec4 STICK_COLOR(0.8f, 0.8f, 0.3f, 1.0f);

DirectionPad::DirectionPad(GameEngine* engine, glm::vec3 center, float size)
    : GameObject(engine, center, PAD_COLOR, size)
    , mStick(engine, center, STICK_COLOR, size/2)
    , mDelta(0.0f)
    , mReleased(true)
{

}

DirectionPad::~DirectionPad()
{

}

void DirectionPad::addListener(DirectionPadListenerPtr listener)
{
    mListener = listener;
}

void DirectionPad::draw()
{
    GameObject::draw(GL_TRIANGLE_FAN);
    mStick.draw(GL_TRIANGLE_FAN);
}

void DirectionPad::onTick(float timeDelta)
{
    GameObject::onTick(timeDelta);

    vec3 stickVector = mPosition - mStick.position();
    float stickDistance = length(stickVector);

    vec3 stickVelocity = vec3(0.0f);
    if (  (stickDistance > boundingRadius() - mStick.boundingRadius() / 2))
    {
        stickVelocity = stickVector;
    }

    if (mReleased && mTime > 0.3f)
    {
        stickVelocity = stickVector / timeDelta;

        if (stickDistance > std::numeric_limits<float>::epsilon())
        {
            stickVelocity /= 20;
        }
        else
        {
            mStick.setPosition(mPosition);
        }
    }
    mStick.setVelocity(stickVelocity);
    mStick.onTick(timeDelta);

    DirectionPadListenerPtr listener = mListener.lock();
    if (listener)
    {
        listener->onPadMove(-stickVector, mDelta, timeDelta);
    }
}

void DirectionPad::moveStick(glm::vec3 position, glm::vec3 delta)
{
    mReleased = false;
    mDelta = delta;
    mStick.setPosition(position);
    mTime = 0.0f;
}

void DirectionPad::release()
{
    mTime = 0.0f;
    mDelta = vec3(0.0f);
    mReleased = true;
}
