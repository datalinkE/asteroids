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
#include "Timer.h"

using namespace glm;
using namespace Geometry;

GLuint squreVBO;
GLuint texture;

mat4 modelMatrix;
mat4 viewMatrix;
mat4 projectionMatrix;

mat4 inverseViewProjectionMatrix;

std::unique_ptr<ShaderProgramColor> shaderProgramColor;
std::unique_ptr<ShaderProgramTexture> shaderProgramTexture;

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

Timer timer;

void on_surface_created()
{
	DLOG();
	timer.Start();
}

void on_surface_changed(int width, int height)
{
	DLOG();
	float aspectRatio = static_cast<float>(width) / height;

	viewMatrix = lookAt(
			vec3(0.0f, 0.0f, 3.0f),  //pos
			vec3(0.0f, 0.0f, 0.0f),  //dir
			vec3(0.0f, 1.0f, 0.0f));  //top

	projectionMatrix = perspective(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
	inverseViewProjectionMatrix = inverse(projectionMatrix * viewMatrix);

	// game coordinates would be [-xMax .. xMax] and [-yMax .. yMax]
	float xMax = 5.0f;
	float yMax = xMax / aspectRatio;

	// fitting x interval to the screen
	// touching the leftmost border should result in xMax
	vec3 unscaled = Touch(1.0f, 0.0f, &inverseViewProjectionMatrix).atPlane(drawPlane);
	float scaleFactor = unscaled[0] / xMax;

	mat4 scaleMatrix = scale(vec3(scaleFactor, scaleFactor, 1.0f));
	viewMatrix = viewMatrix * scaleMatrix;
    inverseViewProjectionMatrix = inverse(scaleMatrix) * inverseViewProjectionMatrix;

    squreVBO = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);

    int circlePoints = 50;
    int circleBufferSize = size_of_circle_in_vertices(circlePoints) * 4;
    float circleBuffer[circleBufferSize];
    gen_circle(circleBuffer, 0.3f, circlePoints);
    circleVBO = GLHelpers::createVBO(sizeof(circleBuffer), circleBuffer, GL_STATIC_DRAW);

    texture = GLHelpers::load_png_asset_into_texture("stone.png");
    shaderProgramColor.reset(new ShaderProgramColor(&viewMatrix, &projectionMatrix));
    shaderProgramTexture.reset(new ShaderProgramTexture(&viewMatrix, &projectionMatrix));
}

float pos = 0.0f;

void on_draw_frame()
{
    timer.Update();
	DLOG() << ARG(timer.GetTimeSim());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pos += timer.GetTimeSim() * 0.1f;
    modelMatrix = translate(vec3(pos, 0.5f, 0.0f));
    //modelMatrix = translate(vec3(0.0f, 0.5f, 0.0f));
    shaderProgramColor->draw(&modelMatrix, circleVBO, vec4(0.0f, 0.0f, 1.0f, 1.0f), GL_TRIANGLE_FAN);

    modelMatrix = translate(vec3(-1.0f, 0.0f, -1.0f));
    shaderProgramColor->draw(&modelMatrix, squreVBO, vec4(0.0f, 1.0f, 0.0f, 1.0f));

    modelMatrix = translate(vec3(1.0f, 0.0f, 0.0f));
    shaderProgramTexture->draw(&modelMatrix, squreVBO, texture);
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
