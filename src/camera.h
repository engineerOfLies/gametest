#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <SDL.h>
#include "vector.h"

SDL_Rect camera_get_active_camera();

Vec2d camera_get_position();
void camera_set_position(Vec2d pos);

void camera_set_size(Vec2d size);
Vec2d camera_get_size();



#endif
