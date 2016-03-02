#include "save.h"

void playerSave(PlayerSave *ps,Events *events,char *filename)
{
    FILE *fp;
    int i;
    if (!ps)return;
    fp = fopen(filename,"wb");
    if (!fp)
    {
        return;
    }
    fwrite(ps,sizeof(PlayerSave),1,fp);
    fwrite(events,sizeof(Events),ps->eventNumber,fp);
    fclose(fp);
}

void playerLoad(PlayerSave *ps,Events **events,char * filename)
{
    FILE *fp;
    if (!ps)return;
    if (!events)return;
    fp = fopen(filename,"rb");
    if (!fp)
    {
        slog("ERROR: failed to open file %s",filename);
        return;
    }

    fread(ps,sizeof(PlayerSave),1,fp);
    
    if (ps->eventNumber > 0)
    {
        *events = (Events *)malloc(sizeof(Events)*ps->eventNumber);
        fread(*events,sizeof(Events),ps->eventNumber,fp);
    }
    
    fclose(fp);
}



/*eol@eof*/
