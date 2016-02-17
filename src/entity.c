#include "entity.h"
#include "simple_logger.h"

static Entity * EntityList = NULL;
static Uint32 MaxEntities = 0;


void closeEntitySystem();

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

/*eol@eof*/