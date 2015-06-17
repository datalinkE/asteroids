#include "GameEngine.h"
#include "Player.h"

#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GLHelpers.h"
#include "Touch.h"

using namespace glm;
using namespace Geometry;

GameEngine::GameEngine()
    : mFontAtlas("Reckoner.ttf", 48)
    , mDragNow(false)
{
    timer.Start();

    drawPlane =
        {
            vec3(0.0f, 0.0f, 0.0f), //point
            vec3(0.0f, 0.0f, 1.0f)  //normal
        };

    // position X, Y, texture S, T
    const float rect[] = {-0.5f, -0.5f, 0.0f, 0.0f,
                         -0.5f,  0.5f, 0.0f, 1.0f,
                          0.5f, -0.5f, 1.0f, 0.0f,
                          0.5f,  0.5f, 1.0f, 1.0f};
    squreVBO = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);

    int circlePoints = 50;
    int circleBufferSize = size_of_circle_in_vertices(circlePoints) * 4;
    float circleBuffer[circleBufferSize];
    gen_circle(circleBuffer, 0.5f, circlePoints);
    circleVBO = GLHelpers::createVBO(sizeof(circleBuffer), circleBuffer, GL_STATIC_DRAW);

    texture = GLHelpers::load_png_asset_into_texture("stone.png");
    shaderProgramColor.reset(new ShaderProgramColor(&viewMatrix, &projectionMatrix));
    shaderProgramTexture.reset(new ShaderProgramTexture(&viewMatrix, &projectionMatrix));
    shaderProgramText.reset(new ShaderProgramText(&viewMatrix, &projectionMatrix));
}

GameEngine::~GameEngine()
{
}

void GameEngine::setGameField(int width, int height)
{
    DLOG();
    float aspectRatio = static_cast<float>(width) / height;

    // game coordinates would be [-xMax .. xMax] and [-yMax .. yMax]
    mXMax = 5.0f;
    mYMax = mXMax / aspectRatio;

    mCollidables.reset(new QuadTree(0, Geometry::Rect {vec2(-mXMax, -mYMax), vec2(mXMax, mYMax)}));

    viewMatrix = lookAt(
            vec3(0.0f, 0.0f, 3.0f),  //pos
            vec3(0.0f, 0.0f, 0.0f),  //dir
            vec3(0.0f, 1.0f, 0.0f));  //top

    projectionMatrix = perspective(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
    inverseViewProjectionMatrix = inverse(projectionMatrix * viewMatrix);

    // fitting x interval to the screen
    // touching the leftmost border should result in xMax
    vec3 unscaled = Touch(1.0f, 0.0f, &inverseViewProjectionMatrix).atPlane(drawPlane);
    float scaleFactor = unscaled[0] / mXMax;

    mat4 scaleMatrix = scale(vec3(scaleFactor, scaleFactor, 1.0f));
    viewMatrix = viewMatrix * scaleMatrix;
    inverseViewProjectionMatrix = inverse(scaleMatrix) * inverseViewProjectionMatrix;
}

void GameEngine::setBaseObjects()
{
    mPlayer = std::make_shared<Player>(this);
    mDirectionPad = std::make_shared<DirectionPad>(this, vec3(3.5f, -6.0f, 0.0f), 2.0f);
    mDirectionPad->addListener(mPlayer);

    mFireButton = std::make_shared<Button>(this, vec3(-3.5f, 6.0f, 0.0f), 2.0f);

    mObjects.insert(mObjects.end(), mPlayer);
}

void GameEngine::boundPlayer()
{
    float boundingAccX = mXMax - std::abs(mPlayer->position()[0]);
    if (boundingAccX > 0.5)
    {
        boundingAccX = 0.0f;
    }
    if (mPlayer->position()[0] > 0)
    {
        if (boundingAccX > 0)
            boundingAccX *= -1;
    }
    else
    {
        if (boundingAccX < 0)
            boundingAccX *= -1;
    }

    float boundingAccY = mYMax - std::abs(mPlayer->position()[1]);
    if (boundingAccY > 0.5f)
    {
        boundingAccY = 0.0f;
    }
    if (mPlayer->position()[1] > 0)
    {
        if (boundingAccY > 0)
            boundingAccY *= -1;
    }
    else
    {
        if (boundingAccY < 0)
            boundingAccY *= -1;
    }

    mPlayer->impulse(glm::vec3(boundingAccX*std::abs(boundingAccX), boundingAccY*std::abs(boundingAccY), 0.0f));
}

void GameEngine::drawAll()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mDirectionPad->draw();
    mFireButton->draw();

    for (GameObjectPtr& object : mObjects)
    {
        object->draw();
    }

    modelMatrix = translate(vec3(-1.0f, 3.0f, 0.0f)) * scale(vec3(0.02f));
    shaderProgramText->draw("asteroids", &modelMatrix, mFontAtlas);

//    modelMatrix = translate(vec3(-1.0f, 0.0f, -1.0f));
//    shaderProgramColor->draw(&modelMatrix, squreVBO, vec4(0.0f, 1.0f, 0.0f, 1.0f));
//
//    modelMatrix = translate(vec3(1.0f, 0.0f, 0.0f));
//    shaderProgramTexture->draw(&modelMatrix, squreVBO, texture);
}

void GameEngine::tick()
{
    timer.Update();
    mCollidables->clear();

    boundPlayer();

    //// moving objects
    for (GameObjectPtr& object : mObjects)
    {
        object->onTick(timer.getSimDelta());
        // unrelated but could be done in the same loop
        if(!object->isDeleted())
        {
            mCollidables->insert(object.get());
        }
    }

    mDirectionPad->onTick(timer.getSimDelta());

    // check for collisions and act on it
    for (GameObjectPtr& object : mObjects)
    {
        if(!object->isDeleted())
        {
            object->doImpacts();
        }
    }

    // actually remove deleted objects
    for (auto it = mObjects.begin(); it != mObjects.end();)
    {
        if((*it)->isDeleted())
        {
            mObjects.erase(it++);
            continue;
        }
        it++;
    }

    drawAll();
}

vec3 GameEngine::touchDrawPlane(float normX, float normY)
{
    Touch touch(normX, normY, &inverseViewProjectionMatrix);
    vec3 posAtDrawPlane = touch.atPlane(drawPlane);
    DLOG() << to_string(posAtDrawPlane);
    return posAtDrawPlane;
}

void GameEngine::inputTap(float normX, float normY)
{
    mDragNow = true;
    DLOG() << ARG(normX) << ARG(normY);
    vec3 pos = touchDrawPlane(normX, normY);
    mDragVector = pos - mDragPoint;
    mDragPoint = pos;

    float distanceToPad = distance(pos, mDirectionPad->position());
    float distanceToButton =  distance(pos, mFireButton->position());
    if (distanceToButton < mFireButton->boundingRadius())
    {
        mFireButton->push();
    }
    else if (distanceToPad < mDirectionPad->boundingRadius() * 3)
    {
        mDirectionPad->moveStick(pos, mDragVector);
    }
    else
    {
        mObjects.insert(mObjects.end(), GameObjectPtr(new GameObject(this, mDragPoint, vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.5f, 5.0f)));
    }
}

void GameEngine::inputRelease(float normX, float normY)
{
    mDragNow = false;
    DLOG() << ARG(normX) << ARG(normY);
    vec3 pos = touchDrawPlane(normX, normY);
    mDragVector = pos - mDragPoint;
    mDragPoint = pos;

    float distanceToPad = distance(pos, mDirectionPad->position());
    if (distanceToPad < mDirectionPad->boundingRadius() * 5)
    {
        mDirectionPad->release();
    }

    float distanceToButton =  distance(pos, mFireButton->position());
    if (distanceToButton < mFireButton->boundingRadius() * 5)
    {
        mFireButton->release();
    }
}

void GameEngine::inputDrag(float normX, float normY)
{
    DLOG() << ARG(normX) << ARG(normY);
    vec3 pos = touchDrawPlane(normX, normY);
    mDragVector = pos - mDragPoint;
    mDragPoint = pos;

    float distanceToPad = distance(pos, mDirectionPad->position());
    if (distanceToPad < mDirectionPad->boundingRadius() * 3)
    {
        mDirectionPad->moveStick(pos, mDragVector);
    }
}

