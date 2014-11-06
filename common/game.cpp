#include "game.h"
#include "Logger.hpp"
#include "GLHelpers.h"

static GLuint buffer;
static GLuint program;
static GLuint texture;

static GLuint a_positionLocation;
static GLuint a_texture_coordinates_location;

static GLuint u_color;
static GLuint u_texture_unit_location;

#define BUFFER_OFFSET(i) ((void*)(i))

// position X, Y, texture S, T
static const float rect[] = {-0.5f, -0.5f, 0.0f, 0.0f,
                             -0.5f,  0.5f, 0.0f, 1.0f,
                              0.5f, -0.5f, 1.0f, 0.0f,
                              0.5f,  0.5f, 1.0f, 1.0f};
//static const float rect[] = {-1.0f, -1.0f, 0.0f, 0.0f,
//                             -1.0f,  1.0f, 0.0f, 1.0f,
//                              1.0f, -1.0f, 1.0f, 0.0f,
//                              1.0f,  1.0f, 1.0f, 1.0f};

void on_surface_created()
{
	DLOG();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void on_surface_changed()
{
//	DLOG();
//    buffer = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);
//    program = GLHelpers::build_program_from_assets("BasicSolidColor.vsh", "BasicSolidColor.fsh");
//    a_positionLocation = glGetAttribLocation(program, "a_Position");
//    u_color = glGetUniformLocation(program, "u_Color");

    texture = GLHelpers::load_png_asset_into_texture("stone.png");
    buffer = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);
    program = GLHelpers::build_program_from_assets("BasicTexture.vsh", "BasicTexture.fsh");

    a_positionLocation = glGetAttribLocation(program, "a_Position");
    a_texture_coordinates_location = glGetAttribLocation(program, "a_TextureCoordinates");
    u_texture_unit_location = glGetUniformLocation(program, "u_TextureUnit");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLHelpers::validate_program(program);
}

void on_draw_frame()
{
//	//DLOG();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glUseProgram(program);
//
//    glUniform4f(u_color, 1.0f, 0.0f, 0.0f, 0.0f);
//
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//
//    glVertexAttribPointer(a_positionLocation, 2, GL_FLOAT, GL_FALSE,
//            4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(a_positionLocation);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glUniform1i(u_texture_unit_location, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(a_positionLocation, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(a_positionLocation);

	glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(a_texture_coordinates_location);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
