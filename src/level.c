#include "level.h"
#include "entity.h"
#include <string.h>
#include <stdio.h>

static Level __level;
extern SDL_Surface *screen;


int touch1to2(cpArbiter *arb, struct cpSpace *space, void *data)
{
  Entity * entB = NULL;
  CP_ARBITER_GET_SHAPES(arb, b, a);
  entB = cpShapeGetUserData(b);
  if ((entB)&&(entB->touch))
  {
    entB->touch(entB,NULL);
  }
  return 1;
}

int touch2to2(cpArbiter *arb, struct cpSpace *space, void *data)
{
  Entity * entA = NULL;
  Entity * entB = NULL;
  CP_ARBITER_GET_SHAPES(arb, a, b);
  entA = cpShapeGetUserData(a);
  entB = cpShapeGetUserData(b);
  if ((entA)&&(entA->touch))
  {
    entA->touch(entA,entB);
  }
  if ((entB)&&(entB->touch))
  {
    entB->touch(entB,entA);
  }
  return 1;
}

int touchany(cpArbiter *arb, struct cpSpace *space, void *data)
{
  printf("touch ?????\n");
  return 1;
}


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
      fprintf(stdout,"LoadLevel: ERROR, could NOT allocate map data\n");
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
    fprintf(stdout,"LoadLevel: ERROR, could NOT open sprite file: %s\n",bgimagepath);
    free(maptemp);
    return;
  }
  temp2 = LoadSprite(tilesetpath,tsw,tsh);
  if (!temp)
  {
    fprintf(stdout,"LoadLevel: ERROR, could NOT open sprite file: %s\n",tilesetpath);
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
  if (__level.space != NULL)
  {
    cpSpaceFree(__level.space);
  }
  __level.space = cpSpaceNew();
  cpSpaceSetGravity(__level.space, cpv(0,0.9));

  cpSpaceSetDefaultCollisionHandler(
    __level.space,
    touchany,
    NULL,
    NULL,
    NULL,
    
    NULL
  );
  
  cpSpaceAddCollisionHandler(
    __level.space,
    2,2,
    touch2to2,
    NULL,
    NULL,
    NULL,
    
    NULL
  );
  cpSpaceAddCollisionHandler(
    __level.space,
    2,1,
    touch1to2,
    NULL,
    NULL,
    NULL,
    
    NULL
  );
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
  if (__level.space != NULL)
  {
    cpSpaceFree(__level.space);
    __level.space = NULL;
  }
}

void InitLevelSystem()
{
  memset(&__level,0,sizeof(Level));
  atexit(CloseLevel);
}

void addEdgeToSpace(cpSpace *space,int x1,int y1,int x2,int y2)
{
  cpShape *edge = NULL;
  edge = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(x1,y1), cpv(x2,y2),1);
  edge->e = 0;
  edge->u = 0;
  cpShapeSetCollisionType(edge, 1);
  cpShapeSetUserData(edge,NULL);
  cpShapeSetLayers(edge,CP_ALL_LAYERS);
  cpSpaceAddShape(space, edge);
}

void addTileToSpace(cpSpace *space,int x,int y,int w,int h)
{
  addEdgeToSpace(space,x,y,x+w,y);
  addEdgeToSpace(space,x,y,x,y+h);
  addEdgeToSpace(space,x+w,y,x+w,y+h);
  addEdgeToSpace(space,x,y+h,x+w,y+h);
}

void SetupTilePhysics()
{
  int j,i;
  if (!__level.space)return;
  if (!__level.tileMap)return;
  for (j = 0;j < __level.tileHeight;++j)
  {
    for (i = 0;i < __level.tileWidth;++i)
    {
      if (__level.tileMap[(j * __level.tileWidth) + i] > 0)
      {
        addTileToSpace(__level.space,i * 32,j*32,__level.tileSet->h,__level.tileSet->h);
      }
    }
  }
}

void AddBodyToLevelSpace(cpBody *body,cpShape *shape)
{
  if (!body)return;
  if (!shape)return;
  if (!__level.space)return;
  cpSpaceAddBody(__level.space,body);
  cpSpaceAddShape(__level.space,shape);
}

void UpdateLevel()
{
  int i;
  if (!__level.space)return;
  for (i = 0; i < 100;++i)
  {
    cpSpaceStep(__level.space, 0.01);
  }
}

cpSpace *GetLevelSpace()
{
  return __level.space;
}

/*eol@eof*/
