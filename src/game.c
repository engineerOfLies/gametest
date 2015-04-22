#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "sprite.h"
#include "mouse.h"
#include "level.h"
#include "graphics.h"
#include "entity.h"
#include "testent.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;

void Init_All();
void HandleInput();

int getImagePathFromFile(char *filepath,char * filename);
int getCoordinatesFromFile(int *x, int *y,char * filename);
void addCoordinateToFile(char *filepath,int x, int y);


/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  Sprite *tile;
  int done;
  int keyn;
  int i;
  int tx = 0,ty = 0;
  SDLMod mods;
  Uint8 *keys;
  Init_All();
  tile = LoadSprite("images/32_32_16_2sprite.png",32,32);
  getCoordinatesFromFile(&tx, &ty,"config.ini");
  addCoordinateToFile("config.ini",7,11);
  if(tile != NULL)
  {
        for(i = 0;i < 12;i++)
        {
            DrawSprite(tile,buffer,(i * tile->w) + tx,ty,0);
        }
  }
  done = 0;
  LoadLevel("levels/testlevel.txt");
  SetupTilePhysics();
  do
  {
    UpdateLevel();
    updateEntityList();
    ResetBuffer ();
    DrawLevel();
    drawEntityList(screen);
    DrawMouse();
    NextFrame();
    SDL_PumpEvents();
    keys = SDL_GetKeyState(&keyn);
    mods = SDL_GetModState();
    if((keys[SDLK_F4])&&(mods & KMOD_ALT))done = 1;
    HandleInput();
  }while(!done);
  exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
  return 0;
}

void HandleInput()
{
  Uint8 buttons;
  int x,y;
  buttons = SDL_GetMouseState(&x,&y);
  if (buttons)
  {
    spawnTestEnt(x, y);
  }
}

void CleanUpAll()
{
  closeEntityList();
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void Init_All()
{
  Init_Graphics();
  InitLevelSystem();
  InitMouse();
  initEntityList();
  atexit(CleanUpAll);
}

int getImagePathFromFile(char *filepath,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int returnValue = -1;
    if (!filepath)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getImagePathFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    if (fscanf(fileptr,"%s",buf))
    {
        if (strcmp(buf,"image:")==0)
        {
            fscanf(fileptr,"%s",filepath);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    return returnValue;
}

void addCoordinateToFile(char *filepath,int x, int y)
{
    FILE *fileptr = NULL;
    if (!filepath)
    {
        fprintf(stdout,"addCoordinateToFile: warning, no input file path provided\n");
        return;
    }
    fileptr = fopen(filepath,"a");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
        return;
    }
    fprintf(fileptr,"newcoordinate: %i %i\n",x,y);
    fclose(fileptr);
}

int getCoordinatesFromFile(int *x, int *y,char * filename)
{
    FILE *fileptr = NULL;
    char buf[255];
    int tx,ty;
    int returnValue = -1;
    if ((!x)&&(!y))
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no output parameter provided\n");
        return -1;
    }
    if (!filename)
    {
        fprintf(stdout,"getCoordinatesFromFile: warning, no input file path provided\n");
        return -1;
    }
    fileptr = fopen(filename,"r");
    if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filename);
        return -1;
    }
    while (fscanf(fileptr,"%s",buf) != EOF)
    {
        if (strcmp(buf,"position:")==0)
        {
            fscanf(fileptr,"%i %i",&tx,&ty);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    if (x)*x = tx;
    if (y)*y = ty;
    return returnValue;
}
