#pragma once

#include <glm/glm.hpp>
#include <memory>

class GameEngine;

class GameObject
{
public:
    GameObject( GameEngine* gameEngine,
                glm::vec3 position = glm::vec3(0.0f),
                float boundingRadius = 1.0f,
                float timeToLive = -1.0f,
                glm::vec3 velocity = glm::vec3(0.0f));

    virtual ~GameObject();

    virtual void move(float timeDelta);
    virtual void interfere();
    virtual void draw();

    bool isDeleted()
    {
        return mDeleted;
    }

protected:
    bool mDeleted;
    float mTimeToLive;
    float mTime;

    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    float mBoundingRadius;

    glm::mat4 mModelMatrix;

    GameEngine* mEngine;
};

typedef std::unique_ptr<GameObject> GameObjectPtr;
