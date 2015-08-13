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
  printf("boom\n");
//  self->state = ES_DYING;
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
  ent->drawOffset.x = -14;
  ent->drawOffset.y = -14;
  ent->sprite = LoadSprite("images/giantbomb.png",32,32);
  ent->update = updateTestEnt;
  ent->touch = testEntTouch;

  ent->body = cpBodyNew(10,cpMomentForCircle(10, 0, 14, cpvzero));
  ent->shape = cpCircleShapeNew(ent->body, 14, cpvzero);

  cpShapeSetLayers(ent->shape,CP_ALL_LAYERS);
  cpBodySetPos(ent->body, cpv(x,y));
  cpShapeSetCollisionType(ent->shape, 2);
  cpShapeSetUserData(ent->shape, (const cpDataPointer)ent);
  AddBodyToLevelSpace(ent->body,ent->shape);
}

/*eol@eof*/
