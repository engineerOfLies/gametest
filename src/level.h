#ifndef __LEVEL__
#define __LEVEL__

#include "sprite.h"

#define LEVELNAMELEN 80


typedef struct
{
  int loaded; /**<true if there is a level loaded*/
  char filename[LEVELNAMELEN]; /**<the filename and path of the level loaded*/
  Sprite *bgImage;  /**<the actual background image for the level*/
  Sprite *tileSet;  /**<the tile sheet for drawing tiles*/
  SDL_Rect bounds;  /**<absolute bounds for entities in this level*/
  int *tileMap; /**<allocated tilemap data*/
  int  tileWidth; /**<width of tiles for tile data*/
  int  tileHeight;/**< height of tile for tile data*/
}Level;


/**
 * @brief loads a game level into memory and sets it up for drawing
 * 
 * @param filename the path and filename of the level to load
 */
void LoadLevel(char *filename);

/**
 * @brief Draws the level currently loaded into memory
 */
void DrawLevel();

/**
 * @brief unloads the level currently in memory
 */
void CloseLevel();

/**
 * @brief sets up level system to a clean state
 */
void InitLevelSystem();
#endif
