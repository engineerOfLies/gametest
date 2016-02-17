#ifndef __VECTOR_H__
#define __VECTOR_H__

/**
 * @brief a simple 2d vector structure
 */
typedef struct
{
    float x;    /**<the x component of the vector*/
    float y;    /**<the y component of the vector*/
}Vect2d;

typedef struct
{
    float x;/**<the x component of the vector*/
    float y;/**<the y component of the vector*/
    float z;/**<the z component of the vector*/
}Vect3d;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}Vect4d;

#define ZERO 0

#define vec2d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y)
#define vec3d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z)
#define vec4d_add(a,b,c) (c.x=a.x+b.x,c.y=a.y+b.y,c.z=a.z+b.z,c.w=a.w+b.w)






#endif
