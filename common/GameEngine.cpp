#include "GameEngine.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GLHelpers.h"
#include "Touch.h"

using namespace glm;
using namespace Geometry;

GameEngine::GameEngine()
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

void GameEngine::tick()
{
    timer.Update();
    mCollidables->clear();

    for (GameObjectPtr& object : mObjects)
    {
        object->move(timer.GetTimeSim());

        if(!object->isDeleted())
        {
            mCollidables->insert(object.get());
        }
    }

    for (GameObjectPtr& object : mObjects)
    {
        if(!object->isDeleted())
        {
            object->interfere();
        }
    }

    for (auto it = mObjects.begin(); it != mObjects.end();)
    {
        if((*it)->isDeleted())
        {
            mObjects.erase(it++);
            continue;
        }

        it++;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (GameObjectPtr& object : mObjects)
    {
        object->draw();
    }

//    modelMatrix = translate(vec3(-1.0f, 0.0f, -1.0f));
//    shaderProgramColor->draw(&modelMatrix, squreVBO, vec4(0.0f, 1.0f, 0.0f, 1.0f));
//
//    modelMatrix = translate(vec3(1.0f, 0.0f, 0.0f));
//    shaderProgramTexture->draw(&modelMatrix, squreVBO, texture);
}

void GameEngine::input(float normalized_x, float normalized_y)
{
    DLOG() << ARG(normalized_x) << ARG(normalized_y);
    Touch touch(normalized_x, normalized_y, &inverseViewProjectionMatrix);

    vec3 posAtDrawPlane = touch.atPlane(drawPlane);
    DLOG() << to_string(posAtDrawPlane);

    mObjects.insert(mObjects.end(), GameObjectPtr(new GameObject(this, posAtDrawPlane, 0.5f, 5.0f)));
}



