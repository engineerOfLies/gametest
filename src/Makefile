##############################################################################
#
# The Linux-GCC Makefile
#
##############################################################################

#
# Object files.
#

OBJ = game.o graphics.o simple_logger.o vector.o sprite.o entity.o

#
# Compiler stuff -- adjust to your system.
#

# Linux

CC     = gcc
SDL_CFLAGS = `sdl2-config --cflags`
SDL_LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer
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

entity.o:entity.c entity.h sprite.h vector.h
sprite.o:sprite.c sprite.h vector.h
vector.o: vector.c vector.h
simple_logger.o: simple_logger.c simple_logger.h
graphics.o: graphics.c graphics.h simple_logger.h
game.o: game.c graphics.h simple_logger.h

#
# The default rule.
#

.c.o:
	gcc $(CFLAGS) $(SDL_CFLAGS) -c $<

