#include "GameObject.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GameEngine.h"

using namespace glm;

GameObject::GameObject(GameEngine* gameEngine, vec3 position)
    : mPosition(position)
    , mVelocity(vec3(0.3f, 0.3f, 0.0f))
    , mBoundingRadius(1.0f)
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
    mPosition += timeDelta * mVelocity;
}

void GameObject::interfere()
{

}

void GameObject::draw()
{
    mModelMatrix = translate(mPosition);
    mEngine->shaderProgramColor->draw(&mModelMatrix, mEngine->circleVBO, vec4(0.0f, 0.0f, 1.0f, 1.0f), GL_TRIANGLE_FAN);
}


