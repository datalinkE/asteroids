#include "game.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.hpp"
#include "GLHelpers.h"
#include "ShaderProgramColor.h"
#include "ShaderProgramTexture.h"
#include "Touch.h"

using namespace glm;
using namespace Geometry;

GLuint buffer;
GLuint texture;

mat4 modelMatrix;
mat4 viewMatrix;
mat4 projectionMatrix;

mat4 inverseViewProjectionMatrix;

std::unique_ptr<ShaderProgram> shaderProgramColor;
std::unique_ptr<ShaderProgram> shaderProgramTexture;

// position X, Y, texture S, T
const float rect[] = {-0.5f, -0.5f, 0.0f, 0.0f,
					 -0.5f,  0.5f, 0.0f, 1.0f,
					  0.5f, -0.5f, 1.0f, 0.0f,
					  0.5f,  0.5f, 1.0f, 1.0f};

GLuint circleVBO;

Plane drawPlane =
{
	vec3(0.0f, 0.0f, 0.0f), //point
	vec3(0.0f, 0.0f, 1.0f)	//normal
};

void on_surface_created()
{
	DLOG();
}

void on_surface_changed(int width, int height)
{
	DLOG();
	viewMatrix = lookAt(
			vec3(0.0f, 0.0f, 3.0f),  //pos
			vec3(0.0f, 0.0f, 0.0f),  //dir
			vec3(0.0f, 1.0f, 0.0f)); //top

	projectionMatrix = perspective(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);

	inverseViewProjectionMatrix = inverse(projectionMatrix * viewMatrix);

    buffer = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);
    texture = GLHelpers::load_png_asset_into_texture("stone.png");
    shaderProgramColor.reset(new ShaderProgramColor(&viewMatrix, &projectionMatrix, vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    shaderProgramTexture.reset(new ShaderProgramTexture(&viewMatrix, &projectionMatrix, texture));

    int circlePoints = 50;
    int circleBufferSize = size_of_circle_in_vertices(circlePoints) * 4;
    float circleBuffer[circleBufferSize];
    gen_circle(circleBuffer, 0.3f, circlePoints);
    circleVBO = GLHelpers::createVBO(sizeof(circleBuffer), circleBuffer, GL_STATIC_DRAW);
}

void on_draw_frame()
{
	//DLOG();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelMatrix = translate(vec3(0.0f, 0.5f, 0.0f));
    shaderProgramColor->draw(&modelMatrix, circleVBO, GL_TRIANGLE_FAN);

    modelMatrix = translate(vec3(-1.0f, 0.0f, -1.0f));
    shaderProgramColor->draw(&modelMatrix, buffer);

    modelMatrix = translate(vec3(1.0f, 0.0f, 0.0f));
    shaderProgramTexture->draw(&modelMatrix, buffer);
}

void on_touch_press(float normalized_x, float normalized_y)
{
	DLOG() << ARG(normalized_x) << ARG(normalized_y);
	Touch touch(normalized_x, normalized_y, &inverseViewProjectionMatrix);
	DLOG() << to_string(touch.atPlane(drawPlane));
}

void on_touch_drag(float normalized_x, float normalized_y)
{
	DLOG() << ARG(normalized_x) << ARG(normalized_y);
}
