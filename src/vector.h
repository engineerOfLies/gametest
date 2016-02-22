#ifndef __VECTOR_H__
#define __VECTOR_H__

/**
 * @brief a simple 2d vector structure
 */
typedef struct
{
    float x;    /**<the x component of the vector*/
    float y;    /**<the y component of the vector*/
}Vec2d;

typedef struct
{
    float x;/**<the x component of the vector*/
    float y;/**<the y component of the vector*/
    float z;/**<the z component of the vector*/
}Vec3d;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}Vec4d;

#define ZERO 0

Vec2d vec2d(float x,float y);

#define vec2d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y)
#define vec3d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z)
#define vec4d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z,c.w=a.w+b.w)


/**
 * @brief check if two rectangles intersect
 */
int  rect_intersect(SDL_Rect a,SDL_Rect b);
SDL_Rect rect(int x,int y,int w,int h);

#endif
