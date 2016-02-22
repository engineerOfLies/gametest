#include <SDL.h>
#include <SDL_image.h>

#include "sprite.h"
#include "simple_logger.h"
#include "graphics.h"

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

void drawSprite(Sprite *sprite,int frame,Vect2d position,SDL_Renderer *renderer)
{
    SDL_Rect cell,target;
    SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
    SDL_Point r;
    Vect2d scaleFactor = {1,1};
    Vect2d scaleOffset = {0,0};
    if (!sprite)
    {
        return;
    }
        
    gt_rect_set(
        &cell,
        frame%sprite->framesPerLine * sprite->frameSize.x,
        frame/sprite->framesPerLine * sprite->frameSize.y,
        sprite->frameSize.x,
        sprite->frameSize.y);
    gt_rect_set(
        &target,
        position.x - (scaleFactor.x * scaleOffset.x),
                 position.y - (scaleFactor.y * scaleOffset.y),
                 sprite->frameSize.x * scaleFactor.x,
                 sprite->frameSize.y * scaleFactor.y);
    SDL_RenderCopyEx(mgl_graphics_get_renderer(),
                     sprite->texture,
                     &cell,
                     &target,
                     rotation?rotation->z:0,
                     rotation?&r:NULL,
                     flipFlags);
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
