#include "camera.h"

static SDL_Rect _Camera = {0,0,0,0};

SDL_Rect camera_get_active_camera()
{
    return _Camera;
}

Vec2d camera_get_position()
{
    return vec2d(_Camera.x,_Camera.y);
}

void camera_set_position(Vec2d pos)
{
    _Camera.x = pos.x;
    _Camera.y = pos.y;
}

void camera_set_size(Vec2d size)
{
    _Camera.w = size.x;
    _camera.h = size.y;
}

Vec2d camera_get_size()
{
    return vec2d(_Camera.w,_Camera.h);
}


/*eol@eof*/
