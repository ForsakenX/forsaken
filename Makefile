#
# General Compiler Settings
#

# for cross compiling be sure to specify your compiler ex:
# make CC=i686-mingw32-gcc
CC=gcc

# general compiler settings
ifeq ($(M32),1)
  FLAGS+= -m32
endif
FLAGS+= -std=gnu99 -pipe -fcommon
CFLAGS=$(FLAGS) -Wall -Wextra
LDFLAGS=$(FLAGS)

# right now non debug build would probably crash anyway
# we even release debug builds as the official release
DEBUG=1

# might as well leave gprof support on by default as well
PROFILE=1

# use this if you want to build everything statically
STATIC=0

# Mudflap is a pointer use checking library. For more info:
# http://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging
MUDFLAP=0

ifeq ($(MUDFLAP),1)
  ifeq ($(DEBUG),1)
    FLAGS+= -fmudflap
    LIB+= -lmudflap
  else
    X:=$(error Mudflap enabled without debug mode - probably not what you meant)
  endif
endif

# stack-smash protection against buffer overflows and corrupt pointers
# (enabled by default on many systems today)
ifeq ($(SSP),1)
  FLAGS+= -fstack-protector-all
  CFLAGS+= -D_FORTIFY_SOURCE=2
endif

ifeq ($(DEBUG),1)
  FLAGS+= -g
else
  CFLAGS+=-O3 -Winit-self
  LDFLAGS+=-s
endif

ifeq ($(PROFILE),1)
  ifneq ($(DEBUG),1)
    # Debug symbols needed for profiling to be useful
    FLAGS+= -g
  endif
  FLAGS+= -pg
endif

#
# ProjectX Specific
#

# some systems use lua5.1
LUA=$(shell pkg-config lua && echo lua || echo lua5.1)
MACOSX=$(shell uname -a | grep -qi darwin && echo 1 || echo 0)

# which version of sdl do you want to ask pkgconfig for ?
SDL=1
ifeq ($(SDL),1)
  SDL_=sdl
else
  SDL_=sdl$(SDL)
endif

# which version of GL do you want to use ?
GL=1

$(if $(shell test "$(GL)" -ge 3 -a "$(SDL)" -lt 2 && echo fail), \
     $(error "GL >= 3 only supported with SDL >= 2"))

# library headers
CFLAGS+= `pkg-config --cflags $(SDL_) $(LUA) $(LUA)-socket libenet libpng zlib openal`
ifeq ($(MACOSX),1)
  CFLAGS += -DMACOSX
endif

# libraries
ifeq ($(STATIC),1)
  ifeq ($(MACOSX),1)
     $(error MacOSX does not support static builds)
  endif
  LIB+= -Wl,-dn
  PKG_CFG_OPTS= --static
endif
LIB+= `pkg-config $(PKG_CFG_OPTS) --libs $(LUA) $(LUA)-socket libenet libpng zlib openal` -lm
ifeq ($(STATIC),1)
  LIB+= -Wl,-dy
endif

# dynamic only libraries
LIB+= `pkg-config --libs $(SDL_)`
ifeq ($(MINGW),1)
  LIB += -L./mingw/bin
  LIB += -lglu32 -lopengl32
  LIB += -lsocket -lws2_32 -lwsock32 -lwinmm -lOpenAL32
else ifeq ($(MACOSX),1)
  # TODO - Support targeting X11 (need to compile sdl properly)
  #LIB += -L/usr/X11/lib/ -lGL -lGLU
  LIB += -framework OpenGL # OpenGL bundle on OSX.
  LIB += -framework Cocoa  # Used to target Quartz by SDL_.
else
  LIB += -lGL -lGLU
endif
ifneq ($(MINGW),1)
  # apparently on some systems -ldl is explicitly required
  # perhaps this is part of the default libs on others...?
  LIB+= -ldl
endif

ifeq ($(BOT),1)
  CFLAGS+= -DLUA_BOT
endif

# ProjectX-specific includes
CFLAGS += -I.

# ProjectX-specific macros
ifeq ($(DXMOUSE),1)
  CFLAGS += -DDXMOUSE -Idinput
  LIB += -Ldinput -ldinput -ldxguid
endif
ifeq ($(RENDER_DISABLED),1)
  CFLAGS+= -DRENDER_DISABLED
else
  CFLAGS+= -DGL=$(GL)
endif
CFLAGS+= -DNET_ENET_2 -DBSP -DLUA_USE_APICHECK -DTEXTURE_PNG -DSOUND_SUPPORT -DSOUND_OPENAL
ifeq ($(DEBUG),1)
  CFLAGS+= -DDEBUG_ON -DDEBUG_COMP -DDEBUG_SPOTFX_SOUND -DDEBUG_VIEWPORT
endif

INC=$(wildcard *.h)
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

# allows user to override settings
ADD_FLAGS=
ADD_CFLAGS=
ADD_LDFLAGS=
FLAGS+=$(ADD_FLAGS)
CFLAGS+=$(ADD_CFLAGS)
LDFLAGS+=$(ADD_LDFLAGS)

BIN=projectx

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS) $(LIB)

$(OBJ): $(INC)

clean:
	$(RM) $(OBJ) $(BIN)

check:
	@echo
	@echo "INC = $(INC)"
	@echo
	@echo "SRC = $(SRC)"
	@echo
	@echo "OBJ = $(OBJ)"
	@echo
	@echo "DEBUG = $(DEBUG)"
	@echo "PROFILE = $(PROFILE)"
	@echo "MUDFLAP = $(MUDFLAP)"
	@echo "STATIC = $(STATIC)"
	@echo "PKG_CFG_OPTS = $(PKG_CFG_OPTS)"
	@echo "MINGW = $(MINGW)"
	@echo "CROSS = $(CROSS)"
	@echo "BOT = $(BOT)"
	@echo "GL = $(GL)"
	@echo "RENDER_DISABLED = $(RENDER_DISABLED)"
	@echo "LUA = $(LUA)"
	@echo "SDL = $(SDL)"
	@echo "SDL_ = $(SDL_)"
	@echo
	@echo "CC = $(CC)"
	@echo "BIN = $(BIN)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIB = $(LIB)"
	@echo

.PHONY: all clean
