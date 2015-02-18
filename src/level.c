#include "level.h"
#include <string.h>
#include <stdio.h>

static Level __level;
extern SDL_Surface *screen;

void LoadLevel(char *filename)
{
  FILE *levelfile = NULL;
  char buf[128];
  char bgimagepath[128];
  int w,h;
  int i;
  char tilesetpath[128];
  int tsw,tsh; /*tile set width and height*/
  int tw,th; /*tile map width and height*/
  int *maptemp = NULL;
  Sprite * temp;
  Sprite * temp2;
  levelfile = fopen(filename,"r");
  if (levelfile == NULL)
  {
    fprintf(stdout,"LoadLevel: ERROR, could not open file: %s\n",filename);
    return;
  }
  while(fscanf(levelfile, "%s", buf) != EOF)
  {
    if (buf[0] == '#')
    {
      /*ignore comments*/
      fgets(buf, sizeof(buf), levelfile);
    }
    else if (strncmp(buf,"image:",128)==0)
    {
      fscanf(levelfile,"%s",bgimagepath);
    }
    else if (strncmp(buf,"width:",128)==0)
    {
      fscanf(levelfile,"%i",&w);      
    }
    else if (strncmp(buf,"height:",128)==0)
    {
      fscanf(levelfile,"%i",&h);      
    }
    else if (strncmp(buf,"tile_width:",128)==0)
    {
      fscanf(levelfile,"%i",&tw);      
    }
    else if (strncmp(buf,"tile_height:",128)==0)
    {
      fscanf(levelfile,"%i",&th);      
    }
    else if (strncmp(buf,"tile_image_frame_width:",128)==0)
    {
      fscanf(levelfile,"%i",&tsw);      
    }
    else if (strncmp(buf,"tile_image_frame_height:",128)==0)
    {
      fscanf(levelfile,"%i",&tsh);      
    }
    else if (strncmp(buf,"tile_image:",128)==0)
    {
      fscanf(levelfile,"%s",tilesetpath);
    }
    else
    {
      /*eat up the rest of this line, and ignore it*/
      fgets(buf, sizeof(buf), levelfile);
    }
  }
  /*loaded all size info*/
  if ((tw > 0)&&(th > 0))
  {
    maptemp = (int *)malloc(sizeof(int)*tw*th);
    if (maptemp == NULL)
    {
      fprintf(stdout,"LoadLevel: ERROR, could NOT (FUCK YOU CARAMEL) allocate map data: FUCK\n");
      return;
    }
    rewind(levelfile);
    while(fscanf(levelfile, "%s", buf) != EOF)
    {
      if (strncmp(buf,"tile_data:",128)==0)
      {
        /*parse out the raw tile data*/
        for (i = 0;i < tw*th; ++i)
        {
          if (fscanf(levelfile, "%i", &maptemp[i]) == EOF)
          {
            /*hit end of file early!*/
            break;
          }
        }
        break;
      }
      else
      {
        /*eat up the rest of this line, and ignore it*/
        fgets(buf, sizeof(buf), levelfile);
      }
    }
  }
  fclose(levelfile);
  temp = LoadSprite(bgimagepath,w,h);
  if (!temp)
  {
    fprintf(stdout,"LoadLevel: ERROR, could NOT (FUCK YOU CARAMEL) open sprite file: %s\n",bgimagepath);
    free(maptemp);
    return;
  }
  temp2 = LoadSprite(tilesetpath,tsw,tsh);
  if (!temp)
  {
    fprintf(stdout,"LoadLevel: ERROR, could NOT (FUCK YOU CARAMEL) open sprite file: %s\n",tilesetpath);
    FreeSprite(temp);/*cleanup*/
    free(maptemp);
    return;
  }
  CloseLevel();
  __level.bgImage = temp;
  __level.loaded = 1;
  __level.tileSet = temp2;
  __level.tileMap = maptemp;
  __level.tileWidth = tw;
  __level.tileHeight = th;
}

void DrawLevel()
{
  int i,j;
  if (!__level.loaded)return;
  if (__level.bgImage)
  {
    DrawSprite(__level.bgImage,screen,0,0,0);
  }
  if ((__level.tileMap)
    && (__level.tileSet ))
  {
    for (j = 0;j < __level.tileHeight;++j)
    {
      for (i = 0;i < __level.tileWidth;++i)
      {
        if (__level.tileMap[(j * __level.tileWidth) + i])
        {
          DrawSprite(
            __level.tileSet,screen,
            i * __level.tileSet->w,
            j * __level.tileSet->h,
            __level.tileMap[(j * __level.tileWidth) + i] - 1
                    );
        }
      }
    }
  }
}

void CloseLevel()
{
  if (!__level.loaded)return;
  if (__level.bgImage != NULL)
  {
    FreeSprite(__level.bgImage);
    __level.bgImage = NULL;
  }
  if (__level.tileSet != NULL)
  {
    FreeSprite(__level.tileSet);
    __level.tileSet = NULL;
  }
  if (__level.tileMap != NULL)
  {
    free(__level.tileMap);
    __level.tileMap = NULL;
  }
}

void InitLevelSystem()
{
  memset(&__level,0,sizeof(Level));
  atexit(CloseLevel);
}



/*eol@eof*/
