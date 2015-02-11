#ifndef __VECTOR__
#define __VECTOR__

#include <SDL.h>

/**
 * @purpose to provide a basic 2D vector structure
 */
typedef struct vec2_s
{
  Sfloat64 x,y;
}vec2_t;

/**
 * @brief adds two vectors together and returns the result
 * 
 * @param v1 one of the adding vectors
 * @param v2 one of the adding vectors
 * 
 * @return the resulting vector
 */
vec2_t vector2_add(vec2_t v1,vec2_t v2);
vec2_t vector2_fast_add(vec2_t v1,vec2_t v2);



#endif
