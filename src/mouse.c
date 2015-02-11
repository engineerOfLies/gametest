#include "mouse.h"
#include "sprite.h"

extern SDL_Surface *screen; /*pointer to the draw buffer*/

Sprite *Msprite;

struct
{
  Uint32 state;
  Uint32 shown;
  Uint32 frame;
  Uint16  x, y;
}Mouse;


/*mouse handling functions*/
/*this only handles the drawing and animation of.  Assuming you have a 16 by 16  tiled sprite sheet.  This will not handle input*/
void InitMouse()
{
  Msprite = LoadSprite("images/mouse.png",16, 16);
  if(Msprite == NULL)fprintf(stdout,"mouse didn't load\n");
  Mouse.state = 0;
  Mouse.shown = 0;
  Mouse.frame = 0;
}

/*draws to the screen immediately before the blit, after all
 *     it wouldn't be a very good mouse if it got covered up by the
 *     game content*/
void DrawMouse()
{
  int mx,my;
  SDL_GetMouseState(&mx,&my);
  if(Msprite != NULL) DrawSprite(Msprite,screen,mx,my,Mouse.frame);
  Mouse.frame = (Mouse.frame + 1)%16;
  Mouse.x = mx;
  Mouse.y = my;
}


/*eol@eof*/
