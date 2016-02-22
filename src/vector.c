#include "vector.h"


Vec2d vec2d(float x,float y)
{
    Vec2d v;
    v.x = x;
    v.y = y;
    return v;
}

SDL_Rect rect(int x,int y,int w,int h)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

int  rect_intersect(SDL_Rect a,SDL_Rect b)
{
    if ((a.x + a.w >= b.x) &&
        (b.x + b.w >= a.x) &&
        (a.y + a.h >= b.y) &&
        (b.y + b.h >= a.y))
        return 1;
    return 0;
}

/*eol@eof*/
