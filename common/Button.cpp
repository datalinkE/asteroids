#include "Button.h"
#include "Logger.hpp"
#include "Geometry.hpp"

using namespace glm;

const vec4 BUTTON_COLOR(0.2f, 0.9f, 0.2f, 0.6f);
const vec4 BUTTON_COLOR_PRESSED(0.9f, 0.2f, 0.2f, 0.6f);

Button::Button(GameEngine* engine, glm::vec3 center, float size)
    : GameObject(engine, center, BUTTON_COLOR, size)
    , mReleased(true)
{

}

Button::~Button()
{

}

void Button::addListener(ButtonListenerPtr listener)
{
    mListener = listener;
}

void Button::draw()
{
    GameObject::draw(GL_TRIANGLE_FAN);
}

void Button::push()
{
    mColor = BUTTON_COLOR_PRESSED;
    mReleased = false;
}

void Button::release()
{
    mColor = BUTTON_COLOR;
    mReleased = true;
}

void Button::onTick(float timeDelta)
{
    ButtonListenerPtr listener = mListener.lock();
    if (listener)
    {
        listener->onPush(timeDelta);
    }
}
