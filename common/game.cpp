#include "game.h"
#include "GameEngine.h"

std::unique_ptr<GameEngine> engine;

void on_surface_created()
{
	DLOG();
	engine.reset(new GameEngine());
}

void on_surface_changed(int width, int height)
{
    engine->setGameField(width, height);
}

void on_draw_frame()
{
    engine->tick();
}

void on_touch_press(float normalized_x, float normalized_y)
{
    engine->input(normalized_x, normalized_y);
}

void on_touch_drag(float normalized_x, float normalized_y)
{
	DLOG() << ARG(normalized_x) << ARG(normalized_y);
}
