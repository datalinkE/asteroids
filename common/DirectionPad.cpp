#include "DirectionPad.h"

using namespace glm;

const vec4 PAD_COLOR(0.5f, 0.3f, 0.0f, 1.0f);
const vec4 STICK_COLOR(0.8f, 0.8f, 0.3f, 1.0f);

DirectionPad::DirectionPad(GameEngine* engine, glm::vec3 center, float size)
    : GameObject(engine, center, PAD_COLOR, size)
    , mStick(engine, center, STICK_COLOR, size/2)
{

}

DirectionPad::~DirectionPad()
{

}

void DirectionPad::addListener(DirectionPadListenerPtr listener)
{

}

void DirectionPad::draw()
{
    GameObject::draw(GL_TRIANGLE_FAN);
    mStick.draw(GL_TRIANGLE_FAN);
}

