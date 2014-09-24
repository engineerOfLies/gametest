#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

extern SDL_Surface *screen;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Rect Camera;

void Init_All();

int getImagePathFromFile(char *filepath,char * filename);
int getCoordinatesFromFile(int *x, int *y,char * filename);
void addCoordinateToFile(char *filepath,int x, int y);


/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
  SDL_Surface *temp = NULL;
  SDL_Surface *bg;
  Sprite *tile;
  int done;
  int keyn;
  int i;
  int mx,my;
  int tx = 0,ty = 0;
  Uint8 *keys;
  char imagepath[512];
  Init_All();
  if (getImagePathFromFile(imagepath,"config.ini") == 0)
  {
    temp = IMG_Load(imagepath);/*notice that the path is part of the filename*/
  }
  if(temp != NULL)						/*ALWAYS check your pointers before you use them*/
    bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  if(bg != NULL)
    SDL_BlitSurface(bg,NULL,buffer,NULL);
  tile = LoadSprite("images/32_32_16_2sprite.png",32,32);
  getCoordinatesFromFile(&tx, &ty,"config.ini");
  fprintf(stdout,"x and y: (%i, %i)\n",tx,ty);
  addCoordinateToFile("config.ini",7,11);
  if(tile != NULL)
  {
        for(i = 0;i < 12;i++)
        {
            DrawSprite(tile,buffer,(i * tile->w) + tx,ty,0);
        }
  }
  done = 0;
  do
  {
    ResetBuffer ();
    DrawMouse();
    NextFrame();
    SDL_PumpEvents();
    keys = SDL_GetKeyState(&keyn);
    if(SDL_GetMouseState(&mx,&my))
    {
      DrawSprite(tile,buffer,(mx /32) * 32,(my /32) * 32,0); 
    }
    if(keys[SDLK_ESCAPE])done = 1;
  }while(!done);
  exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
  return 0;
}

void CleanUpAll()
{
  CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void Init_All()
{
  Init_Graphics();

  InitMouse();
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
        fprintf(stdout,"buf is: %s\n",buf);
        if (strcmp(buf,"position:")==0)
        {
            fscanf(fileptr,"%i %i",&tx,&ty);
            fprintf(stdout,"as read: %i, %i\n",tx,ty);
            returnValue = 0;
        }
    }
    fclose(fileptr);
    if (x)*x = tx;
    if (y)*y = ty;
    return returnValue;
}
