#include "game.h"
#include "Logger.hpp"
#include "GLHelpers.h"

static GLuint buffer;
static GLuint program;
static GLuint a_positionLocation;
static GLuint u_color;

#define BUFFER_OFFSET(i) ((void*)(i))

// position X, Y, texture S, T
static const float rect[] = {-0.5f, -0.5f, 0.0f, 0.0f,
                             -0.5f,  0.5f, 0.0f, 1.0f,
                              0.5f, -0.5f, 1.0f, 0.0f,
                              0.5f,  0.5f, 1.0f, 1.0f};

void on_surface_created()
{
	DLOG();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void on_surface_changed()
{
	DLOG();
    buffer = GLHelpers::createVBO(sizeof(rect), rect, GL_STATIC_DRAW);
    program = GLHelpers::build_program_from_assets("BasicSolidColor.vsh", "BasicSolidColor.fsh");
    a_positionLocation = glGetAttribLocation(program, "a_Position");
    u_color = glGetUniformLocation(program, "u_Color");
}

void on_draw_frame()
{
	DLOG();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glUniform4f(u_color, 1.0f, 0.0f, 0.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(a_positionLocation, 2, GL_FLOAT, GL_FALSE,
            4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(a_positionLocation);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
