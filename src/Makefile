CC = gcc
CXX = g++
RM = rm -f
DEFAULT_CFLAGS = -Wall -O2 -ffast-math `sdl-config --cflags`
CFLAGS = $(DEFAULT_CFLAGS)
CXXFLAGS = $(DEFAULT_CFLAGS)
#LDFLAGS = `sdl-config --libs`
LDFLAGS = -L/usr/local/mingw32/lib -logg -lvorbis -lvorbisenc -lvorbisfile -lsmpeg -lmingw32 -lSDL_gfx -lSDL_mixer -lSDLmain -lSDL -mwindows -mno-cygwin

OBJS = ram.o util_snd.o function.o dconv.o scene.o title.o init.o act.o option.o ending.o logo.o stageselect.o demo.o bush.o
OBJS += charcode/charcode.o credits.o
#OBJS = ram.o util_snd.o function.o dconv.o scene.o title.o init.o act.o option.o ending.o logo.o stageselect.o demo.o

SOURCES = $(patsubst %.o, %.c, $(OBJS))

ifeq ($(wildcard .depend),.depend)
do-it-all: act
include .depend
else
do-it-all: depend recursive
recursive:
	make
endif

.PHONY: depend
depend: $(OBJS)
	$(CC) -MM $(CFLAGS) $(SOURCES) > .depend


.PHONY: clean
clean:
	$(RM) $(OBJS) *~ .depend act.exe

act: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
