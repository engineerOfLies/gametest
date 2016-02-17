#include <SDL_image.h>

#include "sprite.h"
#include "simple_logger.h"

static Sprite * spriteList = NULL;
static Uint32 MaxSprites = 0;


void closeSpriteSystem();

void initSpriteSystem(int maxSprites)
{
    if (maxSprites == 0)
    {
        slog("cannot allocate zero sprites!");
        return;
    }
    spriteList = (Sprite *)malloc(sizeof(Sprite) * maxSprites);
    if (!spriteList)
    {
        slog("failed to allocate sprite system.");
        return;
    }
    memset(spriteList,0,sizeof(Sprite)*maxSprites);
    MaxSprites = maxSprites;
    atexit(closeSpriteSystem);
}

void closeSpriteSystem()
{
    int i;
    if (!spriteList)
    {
        return;
    }
    for (i = 0; i < MaxSprites; i++)
    {
        if (spriteList[i].image != 0)
        {
            SDL_DestroyTexture(spriteList[i].image);
        }
    }
    free(spriteList);
    spriteList = NULL;
    MaxSprites = 0;
}

Sprite *loadSprite(char *filename,int frameW,int frameH)
{
    int i;
    SDL_Surface *surface;
    Sprite *sprite = NULL;
    if (!spriteList)
    {
        slog("error: using sprite system uninitialized");
        return NULL;
    }
    for (i = 0;i < MaxSprites;i++)
    {
        if (spriteList[i].refCount == 0)
        {
            continue;
        }
        if (strcmp(spriteList[i].filename,filename)==0)
        {
            spriteList[i].refCount++;
            return &spriteList[i];
        }
    }
    /*not already loaded, need to load a new one*/
    surface = IMG_Load(filename);
    if (!surface)
    {
        slog("failed to load file %s, re: %s",filename,SDL_GetError());
        return NULL;
    }
    
    
    return sprite;
}

void freeSprite(Sprite **sprite)
{
    Sprite *target;
    if (!sprite)return;
    if (!*sprite)return;
    target = *sprite;
    target->refCount--;
    if (target->refCount <= 0)
    {
        SDL_DestroyTexture(target->image);
        memset(target,0,sizeof(Sprite));
    }
    *sprite = NULL;
}
/*eol@eof*/
