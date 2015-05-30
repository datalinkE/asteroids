#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"

class DirectionPad;
typedef std::shared_ptr<DirectionPad> DirectionPadPtr;

class DirectionPadListener;
typedef std::shared_ptr<DirectionPadListener> DirectionPadListenerPtr;
typedef std::weak_ptr<DirectionPadListener> DirectionPadListenerWPtr;

class DirectionPad : public GameObject
{
public:
    DirectionPad(GameEngine* engine, glm::vec3 center, float size);
    virtual ~DirectionPad();

    void addListener(DirectionPadListenerPtr listener);

    void draw();

    void doImpacts() {};
    void onImpact() {};
private:
    GameObject mStick;
    DirectionPadListenerWPtr mListener;
};

class DirectionPadListener
{
public:
    virtual void onPadMove(glm::vec3 position, glm::vec3 delta, float timeDelta) = 0;
};

