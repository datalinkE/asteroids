#include "game.h"
#include "Logger.hpp"
#include "GLHelpers.h"

void on_surface_created()
{
    DLOG();
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}

void on_surface_changed()
{
    DLOG();
}

void on_draw_frame()
{
    DLOG();
	glClear(GL_COLOR_BUFFER_BIT);
}
