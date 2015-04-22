#include "sprite.h"
#include "graphics.h"

#define MaxSprites    255

static void SwapSprite(SDL_Surface *sprite,int color1,int color2,int color3);

Sprite SpriteList[MaxSprites];
int NumSprites;


/*
 *  InitSpriteList is called when the program is first started.
 *  It just sets everything to zero and sets all pointers to NULL.
 *  It should never be called again.
 */

void InitSpriteList()
{
  int x;
  NumSprites = 0;
  memset(SpriteList,0,sizeof(Sprite) * MaxSprites);
  for(x = 0;x < MaxSprites;x++)SpriteList[x].image = NULL;
}


/*Create a sprite from a file, the most common use for it.*/

Sprite *LoadSprite(char *filename,int sizex, int sizey)
{
  int i;
  SDL_Surface *temp;
  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if(strncmp(filename,SpriteList[i].filename,20)==0)
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= MaxSprites)
  {
    fprintf(stderr, "Maximum Sprites Reached.\n");
    exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }
  SpriteList[i].image = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  /*sets a transparent color for blitting.*/
  SDL_SetColorKey(SpriteList[i].image, SDL_SRCCOLORKEY , SDL_MapRGB(SpriteList[i].image->format, 0,0,0));
  /*then copy the given information to the sprite*/
  strncpy(SpriteList[i].filename,filename,20);
  /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].framesperline = 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].used++;
  return &SpriteList[i];
}

/*the palette swapping version of LoadSprite.  It will check the file loaded to see if there is any pure colors for swapping them.*/

Sprite *LoadSwappedSprite(char *filename,int sizex, int sizey, int c1, int c2, int c3)
{
  int i;
  SDL_Surface *temp;
  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if((strncmp(filename,SpriteList[i].filename,20)==0)&&(SpriteList[i].used >= 1)&&(c1 == SpriteList[i].color1)&&(c2 == SpriteList[i].color2)&&(c3 == SpriteList[i].color3))
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= MaxSprites)
  {
    fprintf(stderr, "Maximum Sprites Reached.\n");
    exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr, "FAILED TO LOAD A VITAL Sprite.\n");
    exit(1);
  }
  SpriteList[i].image = SDL_DisplayFormatAlpha(temp);
  SDL_FreeSurface(temp);
  /*sets a transparent color for blitting.*/
  SDL_SetColorKey(SpriteList[i].image, SDL_SRCCOLORKEY , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
  /*fprintf(stderr,"asked for colors: %d,%d,%d \n",c1,c2,c3);*/
  SwapSprite(SpriteList[i].image,c1,c2,c3);
  /*then copy the given information to the sprite*/
  strcpy(SpriteList[i].filename,filename);
  /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].framesperline = 16;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].color1 = c1;
  SpriteList[i].color2 = c2;
  SpriteList[i].color3 = c3;
  SpriteList[i].used++;
  return &SpriteList[i];
}

/*
 * When we are done with a sprite, lets give the resources back to the system...
 * so we can get them again later.
 */

void FreeSprite(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->used--;
  if(sprite->used == 0)
  {
    strcpy(sprite->filename,"\0");
    /*just to be anal retentive, check to see if the image is already freed*/
    if(sprite->image != NULL)SDL_FreeSurface(sprite->image);
    sprite->image = NULL;
  }
  /*and then lets make sure we don't leave any potential seg faults 
   *  lying around*/
}

void CloseSprites()
{
  int i;
  for(i = 0;i < MaxSprites;i++)
  {
    /*it shouldn't matter if the sprite is already freed, 
     *     FreeSprite checks for that*/
    FreeSprite(&SpriteList[i]);
  }
}

void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame)
{
  SDL_Rect src,dest;
  if ((!sprite)||(!surface))return;
  src.x = frame%sprite->framesperline * sprite->w;
  src.y = frame/sprite->framesperline * sprite->h;
  src.w = sprite->w;
  src.h = sprite->h;
  dest.x = sx;
  dest.y = sy;
  dest.w = sprite->w;
  dest.h = sprite->h;
  SDL_BlitSurface(sprite->image, &src, surface, &dest);  
}

static void SwapSprite(SDL_Surface *sprite,int color1,int color2,int color3)
{
  int x, y;
  Uint32 pixel;
  /*First the precautions, that are tedious, but necessary*/
  if(color1 == -1)return;
  if(sprite == NULL)return;
  if ( SDL_LockSurface(sprite) < 0 )
  {
    fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
    exit(1);
  }
  /*now step through our sprite, pixel by pixel*/
  for(y = 0;y < sprite->h ;y++)
  {
    for(x = 0;x < sprite->w ;x++)
    {                           
      pixel = getpixel(sprite,x,y);/*and swap it*/
      putpixel(sprite,x,y,SetColor(pixel,color1,color2,color3));
    }
  }
  SDL_UnlockSurface(sprite);
}



/*eol@eof*/
