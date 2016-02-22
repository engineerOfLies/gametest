#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL.h>
#include "vector.h"

typedef struct
{
   int refCount;        /**<keeps track of references to this element*/
   char filename[120];  /**<name of the image file loaded*/
   SDL_Texture * image;   /**<texture of the image loaded*/
   Vec2d imageSize;     /**<x and y dimensions of the image loaded*/
   int framesPerLine;        /**<how many frames per line in this sprite sheet*/
   Vec2d frameSize;    /**<the dimensions of a frame in the sprite sheet*/
}Sprite;

void initSpriteSystem(int maxSprites);

Sprite *loadSprite(char *filename,int frameW,int frameH);
void drawSprite(Sprite *sprite,int frame,Vec2d position);

/**
 * @brief frees a loaded sprite from memory
 * @param sprite the sprite to be freed
 */
void freeSprite(Sprite **sprite);

#endif
