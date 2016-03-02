#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "sprite.h"


typedef struct
{
    char    name[128];
    Sprite *tileSet;
    int    *tiles;
    int     tileWidth;
    int     tileHeight;
    int     tileCount;
    Vec2d   tileSize;       /**<size of a standard tile*/
    Sprite *backgroung;     /**<static background image*/
}Level;

/**
 * @brief load a level from a config file
 * @param filepath the file to load a tile from
 * @return NULL on failure to load or error, a Level pointer otherwise
 */
Level * level_load(char *filepath);

void  level_draw(Level *level);

void  level_free(Level **level);

#endif
