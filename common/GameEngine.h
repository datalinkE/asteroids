#pragma once

#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "ShaderProgramColor.h"
#include "ShaderProgramTexture.h"
#include "Geometry.hpp"
#include "Timer.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "ShaderProgramText.h"
#include "FontAtlas.h"

class GameEngine
{
public:
    GameEngine();

    ~GameEngine();

    void setGameField(int width, int height);

    void tick();

    void input(float normalized_x, float normalized_y);

    friend class GameObject;

private:

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::mat4 inverseViewProjectionMatrix;

    std::unique_ptr<ShaderProgramColor> shaderProgramColor;
    std::unique_ptr<ShaderProgramTexture> shaderProgramTexture;
    std::unique_ptr<ShaderProgramText> shaderProgramText;

    GLuint squreVBO;
    GLuint circleVBO;

    GLuint texture;

    Geometry::Plane drawPlane;
    Timer timer;

    std::list<GameObjectPtr> mObjects;

    float mXMax;
    float mYMax;

    std::unique_ptr<QuadTree> mCollidables;

    FontAtlas mFontAtlas;
};
