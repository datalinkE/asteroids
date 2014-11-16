#pragma once

#include <glm/glm.hpp>

class GameEngine;

class GameObject
{
public:
    GameObject(GameEngine* gameEngine, glm::vec3 position = glm::vec3(0.0f));
    virtual ~GameObject();

    virtual void move(float timeDelta);
    virtual void interfere();
    virtual void draw();

private:
    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    float mBoundingRadius;

    glm::mat4 mModelMatrix;

    GameEngine* mEngine;
};

