#pragma once

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"

class Button;
typedef std::shared_ptr<Button> ButtonPtr;

class ButtonListener;
typedef std::shared_ptr<ButtonListener> ButtonListenerPtr;
typedef std::weak_ptr<ButtonListener> ButtonListenerWPtr;

class Button : public GameObject
{
public:
    Button(GameEngine* engine, glm::vec3 center, float size);
    virtual ~Button();

    void addListener(ButtonListenerPtr listener);

    void onTick(float timeDelta);

    void draw();

    void doImpacts() {};
    void onImpact() {};

    void push();
    void release();

private:
    bool mReleased;
    ButtonListenerWPtr mListener;
};

class ButtonListener
{
public:
    virtual void onPush(float timeDelta) = 0;
    virtual ~ButtonListener() {};
};


