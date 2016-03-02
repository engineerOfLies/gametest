#include "entity.h"
#include "simple_logger.h"

static Entity * EntityList = NULL;
static Uint32 MaxEntities = 0;


void closeEntitySystem();
int entity_intersect(Entity *a, Entity *b);
int entity_intersect_rect(Entity *a,SDL_Rect r);

void initEntitySystem(int maxEntities)
{
    if (maxEntities == 0)
    {
        slog("cannot allocate zero Entities!");
        return;
    }
    EntityList = (Entity *)malloc(sizeof(Entity) * maxEntities);
    if (!EntityList)
    {
        slog("failed to allocate Entity system.");
        return;
    }
    memset(EntityList,0,sizeof(Entity)*maxEntities);
    MaxEntities = maxEntities;
    atexit(closeEntitySystem);
}

void closeEntitySystem()
{
    int i;
    Entity *ent;
    for (i = 0; i < MaxEntities;i++)
    {
        ent= &EntityList[i];
        entity_free(&ent);
    }
    free(EntityList);
}


Entity *entity_new()
{
    int i;
    for (i = 0; i < MaxEntities;i++)
    {
        if (EntityList[i].inuse)
        {
            continue;
        }
        memset(&EntityList[i],0,sizeof(Entity));
        /*set some default values here*/
        EntityList[i].inuse = 1;
        return &EntityList[i];
    }
    return NULL;
}

void entity_free(Entity **entity)
{
    Entity *self;
    if (!entity)return;
    if (!*entity)return;
    self = *entity;
    freeSprite(&self->sprite);
    *entity = NULL;
}

void entity_draw(Entity *ent,SDL_Renderer *render)
{
    Vec2d position;
    SDL_Rect camera;
    if ((!ent)||(!render))
    {
        slog("error: no entity or renderer passed");
        return;
    }
    if (!ent->sprite)
    {
        slog("error: entity %s has no sprite!",ent->name);
        return;
    }
    camera = camera_get_active_camera();
    if (!entity_intersect_rect(ent,camera))
    {
        return;
    }
    if (ent->cameraEnt)
    {
        position.x = ent->position.x - camera.x;
        position.y = ent->position.y - camera.y;
    }
    drawSprite(ent->sprite,ent->frame,position);
}

void entity_draw_all()
{
    int i;
    for (i = 0; i < MaxEntities;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        
        
    }
}

void entity_think_all()
{
    int i;
    for (i = 0; i < MaxEntities;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        
        if (!EntityList[i].think)
        {
            continue;
        }
        
        EntityList[i].think(&EntityList[i]);
    }
}

void entity_update_all()
{
    int i;
    for (i = 0; i < MaxEntities;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        
        vec2d_add(EntityList[i].position,EntityList[i].velocity,EntityList[i].position);
        
        if (!EntityList[i].update)
        {
            continue;
        }
        
        EntityList[i].update(&EntityList[i]);
    }
}

Entity *entity_intesect_all(Entity *a)
{
    int i;
    if (!a)return NULL;
    for (i = 0; i < MaxEntities;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        if (a == &EntityList[i])
        {
            continue;
            /*don't clip self*/
        }
        if (entity_intersect(a, &EntityList[i]))
        {
            return &EntityList[i];
        }
    }
    return NULL;
}

int entity_intersect_rect(Entity *a,SDL_Rect r)
{
    SDL_Rect aB;
    if (!a)
    {
        slog("ERROR: Missing entity for check");
        return 0;
    }
    aB = rect(
        a->position.x + a->bounds.x,
        a->position.y + a->bounds.y,
        a->bounds.w,
        a->bounds.h);
    return rect_intersect(aB,r);
}

int entity_intersect(Entity *a, Entity *b)
{
    SDL_Rect aB,bB;
    if ((!a) || (!b))
    {
        slog("ERROR: Missing entity for check");
        return 0;
    }
    aB = rect(
        a->position.x + a->bounds.x,
        a->position.y + a->bounds.y,
        a->bounds.w,
        a->bounds.h);
    bB = rect(
        b->position.x + b->bounds.x,
        b->position.y + b->bounds.y,
        b->bounds.w,
        b->bounds.h);
    return rect_intersect(aB,bB);
}

/*eol@eof*/