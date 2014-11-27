#pragma once

#include <glm/glm.hpp>
#include <memory>

class GameEngine;

class GameObject
{
public:
    GameObject( GameEngine* gameEngine,
                glm::vec3 position = glm::vec3(0.0f),
                glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                float boundingRadius = 1.0f,
                float timeToLive = -1.0f,
                glm::vec3 velocity = glm::vec3(0.0f));

    virtual ~GameObject();

    virtual void move(float timeDelta);
    virtual void interfere();
    virtual void draw();

    void impulse(const glm::vec3& delta);

    const glm::vec3& position() const
    {
        return mPosition;
    }

    float boundingRadius() const
    {
        return mBoundingRadius;
    }

    bool isDeleted() const
    {
        return mDeleted;
    }

    bool interferedThisTick() const
    {
        return mInterfered;
    }

protected:
    bool mDeleted;
    bool mInterfered;
    float mTimeToLive;
    float mTime;

    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    float mBoundingRadius;

    glm::mat4 mModelMatrix;

    GameEngine* mEngine;

    glm::vec4 mColor;
};

typedef std::shared_ptr<GameObject> GameObjectPtr;
