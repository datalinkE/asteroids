#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


void on_surface_created();

void on_surface_changed(int width, int height);

void on_draw_frame();


#if defined(__cplusplus)
}
#endif
