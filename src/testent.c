#include "testent.h"
#include "entity.h"
#include "level.h"

void testEntTouch(Entity *self,Entity *other)
{
  if (!self)return;
  if ((self->state == ES_DEAD)||(self->state == ES_DYING))
  {
    return;
  }
  self->state = ES_DYING;
}

void updateTestEnt(Entity *ent)
{
  cpVect v;
  if (!ent)return;
  ent->frame = (ent->frame +1)%16;
  v = cpBodyGetPos(ent->body);
  ent->position.x = v.x;
  ent->position.y = v.y;
  if (ent->state == ES_DYING)
  {
    freeEntity(&ent);
  }
}

void spawnTestEnt(int x, int y)
{
  Entity *ent;
  ent = newEntity();
  if (!ent)return;
  ent->position.x = x;
  ent->position.y = y;
  ent->sprite = LoadSprite("images/giantbomb.png",32,32);
  ent->body = cpBodyNew(10,cpMomentForCircle(10, 16, 0, cpvzero));
  ent->update = updateTestEnt;
  ent->shape = cpCircleShapeNew(ent->body, 16, cpvzero);
  cpShapeSetLayers(ent->shape,CP_ALL_LAYERS);
  cpBodySetPos(ent->body, cpv(x,y));
  cpBodySetUserData(ent->body, (const cpDataPointer)ent);
  AddBodyToLevelSpace(ent->body,ent->shape);
}

/*eol@eof*/
