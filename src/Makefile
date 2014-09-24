##############################################################################
#
# The Linux-GCC Makefile
#
##############################################################################

#
# Object files.
#

OBJ = game.o graphics.o 

#
# Compiler stuff -- adjust to your system.
#

# Linux

CC     = gcc
SDL_CFLAGS = `sdl-config --cflags`
SDL_LDFLAGS = `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer
LFLAGS = -g -o ../gametest
CFLAGS = -g -Wall -pedantic

#
# Targets.
#

lavos: $(OBJ)
	gcc $(OBJ) $(LFLAGS) $(SDL_LDFLAGS)
  
depend:
	@-rm makefile.dep
	@echo Creating dependencies.
	@for i in *.c; do $(CC) $(INC) -MM $$i; done > makefile.dep
	@echo Done.

clean:
	rm *.o gametest

count:
	wc *.c *.h makefile

#
# Dependencies.
#

graphics.o: graphics.c graphics.h
game.o: game.c graphics.h

#
# The default rule.
#

.c.o:
	gcc $(CFLAGS) $(SDL_CFLAGS) -c $<

