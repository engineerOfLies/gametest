#ifndef __ENTITY__
#define __ENTITY__

#include <chipmunk/chipmunk.h>
#include "sprite.h"
#include "vector.h"

typedef enum {
  ES_ALIVE = 0,
  ES_DYING = 1,
  ES_DEAD = 2
}EntStates;

typedef struct entity_s
{
    int inuse;
    Sprite *sprite;
    int frame;
    EntStates state;
    vec2_t position;
    vec2_t drawOffset;
    vec2_t velocity;
    SDL_Rect bbox;
    float radius;
    void (*think)(struct entity_s *self);/**<pointer to the think function for this entity*/
    void (*update)(struct entity_s *self);/**<pointer to the update function for this entity*/
    void (*die)(struct entity_s *self);/**<pointer to the function called when the entity dies*/
    void (*touch)(struct entity_s *self,struct entity_s *other);/**<pointer to the function called when the entity touches another entity*/
    void (*world_touch)(struct entity_s *self,vec2_t contact_point);/**<function to call when an entity touches the world*/
    struct entity_s * target; /**<Pointer to the target entity*/
    struct entity_s * owner; /**<Pointer to the target entity*/
    
    /*physics section*/
    
    cpBody *body;
    cpShape *shape;
}Entity;

void initEntityList();
void closeEntityList();
void drawEntityList(SDL_Surface * surface);
void updateEntityList();
void thinkEntityList();
Entity *newEntity();
void freeEntity(Entity **ent);


#endif /*include guards*/
