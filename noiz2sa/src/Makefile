TARGET = noiz2sa
PSPSDK = $(shell psp-config --pspsdk-path)
PSPBIN = $(shell psp-config --psp-prefix)/bin
SDL_CONFIG = $(PSPBIN)/sdl-config

# Set this to 1 to resize the game to fit the PSP's screen
PSP_SCREEN = 0

O = o
OBJS =	$(TARGET).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
	background.$(O) letterrender.$(O) \
	screen.$(O) clrtbl.$(O) vector.$(O) degutil.$(O) rand.$(O) soundmanager.$(O)

DEFAULT_CFLAGS = $(shell $(SDL_CONFIG) --cflags)

MORE_CFLAGS = -O2 -fsingle-precision-constant -ffast-math -DPSP_SCREEN=$(PSP_SCREEN)

CFLAGS = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
CXXFLAGS = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -fno-exceptions

LIBS = -lbulletml -lSDL_mixer -lvorbisidec $(shell $(SDL_CONFIG) --libs) -lstdc++

EXTRA_TARGETS = EBOOT.PBP

include $(PSPSDK)/lib/build.mak
