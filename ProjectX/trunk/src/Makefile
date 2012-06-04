#
# General Compiler Settings
#

CC=gcc

FLAGS= -m32 -pipe
CFLAGS=$(FLAGS) -Wall -Wextra
LDFLAGS=$(FLAGS)

DEBUG=0
PROFILE=0
STATIC=0
MUDFLAP=0

ifeq ($(MINGW),1)
  ifeq ($(CROSS),1)
    CC=i686-mingw32-gcc
  endif
endif

ifeq ($(MUDFLAP),1)
  ifeq ($(DEBUG),1)
    FLAGS+= -fmudflap
    LIB+= -lmudflap
  else
    X:=$(error Mudflap enabled without debug mode - probably not what you meant)
  endif
endif

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
LUA=$(shell pkg-config lua5.1 && echo lua5.1 || echo lua)

# library headers
CFLAGS+= `pkg-config --cflags sdl $(LUA) $(LUA)-socket libenet libpng zlib openal`

# libraries
ifeq ($(STATIC),1)
  LIB+= -Wl,-dn
  PKG_CFG_OPTS= --static
endif
LIB+= -lm `pkg-config $(PKG_CFG_OPTS) --libs $(LUA) $(LUA)-socket libenet libpng zlib openal`
ifeq ($(STATIC),1)
  LIB+= -Wl,-dy
endif

# dynamic only libraries
LIB+= `pkg-config --libs sdl`
ifeq ($(MINGW),1)
  LIB += -L./mingw/bin
  LIB += -lglu32 -lopengl32
  LIB += -lsocket -lws2_32 -lwsock32 -lwinmm -lOpenAL32 
else
  LIB += -lGL -lGLU
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
	@echo
	@echo "CC = $(CC)"
	@echo "BIN = $(BIN)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIB = $(LIB)"
	@echo

.PHONY: all clean
