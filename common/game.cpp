#include "game.h"
#include "GameEngine.h"

std::unique_ptr<GameEngine> engine;

void on_surface_created()
{
	DLOG();
}

void on_surface_changed(int width, int height)
{
    DLOG();
    static bool once = false;
    if (!once)
    {
        engine.reset(new GameEngine());
        engine->setGameField(width, height);
        engine->setBaseObjects();
        once = true;
    }
}

void on_draw_frame()
{
    engine->tick();
}

void on_touch_down(float normalized_x, float normalized_y)
{
    engine->inputTap(normalized_x, normalized_y);
}

void on_touch_up(float normalized_x, float normalized_y)
{
    engine->inputRelease(normalized_x, normalized_y);
}

void on_touch_drag(float normalized_x, float normalized_y)
{
    engine->inputDrag(normalized_x, normalized_y);
}
