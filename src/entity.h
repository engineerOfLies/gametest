#ifndef __ENTITY__
#define __ENTITY__

typedef struct entity_s
{
    int inuse;
    Sprite *sprite;
    int frame;
    vec2_t position;
    vec2_t velocity;
    SDL_Rect bbox;
    float radius;
    void (*think)(struct entity_s *self);/**<pointer to the think function for this entity*/
    void (*die)(struct entity_s *self);/**<pointer to the function called when the entity dies*/
    void (*touch)(struct entity_s *self,struct entity_s *other);/**<pointer to the function called when the entity touches another entity*/
    void (*world_touch)(struct entity_s *self,vec2_t contact_point);/**<function to call when an entity touches the world*/
    struct entity_s * target; /**<Pointer to the target entity*/
    struct entity_s * owner; /**<Pointer to the target entity*/
}Entity;

void initEntityList();
void closeEntityList();
void drawEntityList();
void updateEntityList();
void thinkEntityList();
Entity *newEntity();
void freeEntity(Entity *ent);


#endif /*include guards*/
