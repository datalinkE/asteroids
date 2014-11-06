#include "game.h"

#include <memory>
#include <glm/glm.hpp>
#include "Logger.hpp"
#include "GLHelpers.h"
#include "ShaderProgramColor.h"
#include "ShaderProgramTexture.h"

using namespace glm;

static GLuint buffer;
static GLuint texture;

mat4 modelMatrix;
mat4 viewMatrix;
mat4 projectionMatrix;

std::unique_ptr<ShaderProgram> shaderProgramColor;
std::unique_ptr<ShaderProgram> shaderProgramTexture;

// position X, Y, texture S, T
static const float rect[] = {-0.5f, -0.5f, 0.0f, 0.0f,
                             -0.5f,  0.5f, 0.0f, 1.0f,
                              0.5f, -0.5f, 1.0f, 0.0f,
                              0.5f,  0.5f, 1.0f, 1.0f};

void on_surface_created()
{
	DLOG();
}

void on_surface_changed()
{
	DLOG();
    buffer = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);
    texture = GLHelpers::load_png_asset_into_texture("stone.png");
    shaderProgramColor.reset(new ShaderProgramColor(viewMatrix, projectionMatrix, vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    shaderProgramTexture.reset(new ShaderProgramTexture(viewMatrix, projectionMatrix, texture));
}

void on_draw_frame()
{
	//DLOG();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //shaderProgramColor->draw(modelMatrix, buffer);
    shaderProgramTexture->draw(modelMatrix, buffer);
}
