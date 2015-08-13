#include "entity.h"
#include "level.h"

void deleteEntity(Entity *ent);

Entity * __entityList = NULL;
int __MaxEntities = 100;

void initEntityList()
{
    /* load entity config from file...*/
    __entityList = (Entity *)malloc(sizeof(Entity)*__MaxEntities);
    if (__entityList == NULL)
    {
        fprintf(stderr,"initEntityList: FATAL: cannot allocate entity list\n");
        exit(-1);
        return;
    }
    memset(__entityList,0,sizeof(Entity)*__MaxEntities);
}

void closeEntityList()
{
  int i;
  for (i = 0;i < __MaxEntities;++i)
  {
    deleteEntity(&__entityList[i]);
  }
}

void drawEntityList(SDL_Surface * surface)
{
  int i;
  for (i = 0;i < __MaxEntities;++i)
  {
    if (!__entityList[i].inuse)
    {
      continue;
    }
    DrawSprite(
      __entityList[i].sprite,
      surface,
      __entityList[i].position.x + __entityList[i].drawOffset.x,
      __entityList[i].position.y + __entityList[i].drawOffset.y,
      __entityList[i].frame);
  }
}

void updateEntityList()
{
  int i;
  for (i = 0;i < __MaxEntities;++i)
  {
    if ((!__entityList[i].inuse)||(!__entityList[i].update))
    {
      continue;
    }
    __entityList[i].update(&__entityList[i]);
  }
}

void thinkEntityList()
{
  int i;
  for (i = 0;i < __MaxEntities;++i)
  {
    if ((!__entityList[i].inuse)||(!__entityList[i].think))
    {
      continue;
    }
    __entityList[i].think(&__entityList[i]);
  }
}

Entity *newEntity()
{
  int i;
  for (i = 0; i < __MaxEntities; i++)
  {
    if (__entityList[i].inuse == 0)
    {
      __entityList[i].inuse = 1;
      return &__entityList[i];
    }
  }
  return NULL;
}

void deleteEntity(Entity *ent)
{
  /*handle freeing resources like Sprite data*/
  if (ent->sprite != NULL)
  {
    FreeSprite(ent->sprite);
  }
  if (ent->shape != NULL)
  {
    cpSpaceRemoveShape(GetLevelSpace(), ent->shape);
    cpShapeFree(ent->shape);
  }
  if (ent->body != NULL)
  {
    cpSpaceRemoveBody(GetLevelSpace(), ent->body);
    cpBodyFree(ent->body);
  }
  memset(ent,0,sizeof(Entity));
}

void freeEntity(Entity **ent)
{
  deleteEntity(*ent);
    *ent = NULL;
}



/*eol@eof*/
