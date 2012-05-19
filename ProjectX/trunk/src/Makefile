# Written by The Lion
#
# The contents of this Makefile is in the public domain.

DEBUG=0
PROFILE=0
STATIC=0

CC=gcc

# Mudflap is a pointer use checking library. For more info:
# http://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging
MUDFLAP=0

# Unused -- this would check for existence of Mudflap.
#HAVE_MUDFLAP=0
#ifneq ($(shell gcc -print-file-name=libmudflap.so), libmudflap.so)
#	HAVE_MUDFLAP=1
#endif

FLAGS= -m32 -pipe
CFLAGS=$(FLAGS) -Wall -Wextra
LDFLAGS=$(FLAGS)

ifeq ($(DEBUG),1)
	FLAGS+= -g 
else
	CFLAGS+=-O3 -Winit-self
	LDFLAGS+=-s
endif

ifeq ($(SSP),1)
	FLAGS+= -fstack-protector-all
	CFLAGS+= -D_FORTIFY_SOURCE=2
endif

SDL_CONFIG=sdl-config

ifeq ($(MINGW),1)

ifeq ($(CROSS),1)
CC=i586-mingw32msvc-gcc
SDL_CONFIG=./mingw/bin/sdl-config
CFLAGS += -I./mingw/include
LIBS += -L./mingw/bin
endif

CFLAGS += -DMINGW `$(SDL_CONFIG) --cflags`
LIBS += -lm -lglu32 -lopengl32 -lenet `$(SDL_CONFIG) --libs`
LIBS += -llua -lsocket -lmime -lluasocket -lws2_32 -lwsock32 -lpng14 -lz -lwinmm -lOpenAL32 

ifeq ($(DXMOUSE),1)
CFLAGS += -DDXMOUSE -Idinput
LIBS +=  -Ldinput -ldinput -ldxguid
endif

else

LUA=$(shell pkg-config lua5.1 && echo lua5.1 || echo lua)

CFLAGS+= -I/usr/include/SDL `pkg-config --cflags $(LUA) $(LUA)-socket` -I/usr/include/AL

LIBS=-lSDL -lGL -lGLU -lm -lpng12 -lz -lopenal

ifeq ($(STATIC),1)
  LIBS= -Wl,-dn
endif

LIBS= -lenet `pkg-config --libs $(LUA) $(LUA)-socket`

ifeq ($(STATIC),1)
  LIBS= -Wl,-dy
endif

endif

ifeq ($(PROFILE),1)
	ifneq ($(DEBUG),1)
		# Debug symbols needed for profiling to be useful
		FLAGS+= -g
	endif
	FLAGS+= -pg
endif

ifeq ($(MUDFLAP),1)
	ifeq ($(DEBUG),1)
		FLAGS+= -fmudflap
		LIBS+= -lmudflap
	else
		X:=$(error Mudflap enabled without debug mode - probably not what you meant)
	endif
endif

ifeq ($(BOT),1)
	CFLAGS+= -DLUA_BOT
endif

# ProjectX-specific includes
CFLAGS += -I.

# ProjectX-specific macros
ifeq ($(RENDER_DISABLED),1)
  CFLAGS+= -DRENDER_DISABLED
else
  ifeq ($(GL),3)
	  CFLAGS+= -DOPENGL3
  else
	  CFLAGS+= -DOPENGL1
  endif
  CFLAGS+= -DOPENGL
endif
CFLAGS+= -DNET_ENET_2 -DBSP -DLUA_USE_APICHECK -DTEXTURE_PNG -DSOUND_SUPPORT -DSOUND_OPENAL
ifeq ($(DEBUG),1)
	CFLAGS+= -DDEBUG_ON -DDEBUG_COMP -DDEBUG_SPOTFX_SOUND -DDEBUG_VIEWPORT 
endif

INCLUDE=$(wildcard *.h)
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))

ADD_FLAGS=
ADD_CFLAGS=
ADD_LDFLAGS=
FLAGS+=$(ADD_FLAGS)
CFLAGS+=$(ADD_CFLAGS)
LDFLAGS+=$(ADD_LDFLAGS)

BIN=projectx

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE)

clean:
	$(RM) $(OBJS) $(BIN)

check:
	@echo "LUA = $(LUA)"
	@echo "LIBS = $(LIBS)"
	@echo "CFLAGS = $(CFLAGS)"

.PHONY: all clean
