#ifndef __SAVE_H__
#define __SAVE_H__

typedef struct
{
    char spriteFile[128];
    int frame;
    vec2_t position;
    char playerName[64];
    int playerClass,playerRace;
    int health,hmax;
    int stamina,smax;
    int exp, level;
    int str,end,intel,agi,dec,resolve,piety;
    char levelName[64];
    int eventNumber;
}PlayerSave;

typedef struct
{
    char level[64];
    char entity[32];
    char status[32];
}Events;

#endif
