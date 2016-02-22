##############################################################################
#
# The Linux-GCC Makefile
#
##############################################################################

#
# Object files.
#

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))

#
# Compiler stuff -- adjust to your system.
#

# Linux

PROJECT = gametest
CC     = gcc
SDL_CFLAGS = `sdl2-config --cflags`
SDL_LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LFLAGS = -g -o ../$(PROJECT)
CFLAGS = -g -Wall -pedantic

#
# Targets.
#

makefile.dep: depend

  
depend:
	@touch makefile.dep
	@-rm makefile.dep
	@echo Creating dependencies.
	@for i in *.c; do $(CC) $(INC) -MM $$i; done > makefile.dep
	@echo Done.

clean:
	rm *.o $(PROJECT)

count:
	wc *.c *.h makefile

#
# Dependencies.
#

include makefile.dep

#
# The default rule.
#

.c.o:
	gcc $(CFLAGS) $(SDL_CFLAGS) -c $<

