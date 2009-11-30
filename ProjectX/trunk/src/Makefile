# Written by The Lion
#
# The contents of this Makefile is in the public domain.

DEBUG=0

CC=gcc

FLAGS=-pipe -march=native
CFLAGS=$(FLAGS)
LDFLAGS=$(FLAGS)
ifeq ($(DEBUG),1)
	FLAGS+=-g
	CFLAGS+=-Wall
else
	CFLAGS+=-O2
	LDFLAGS+=-s
endif
LIBS=-lSDL -lGL -lGLU -llua -llua-mime -llua-socket -lm -lenet -lpng12

# ProjectX-specific macros
CFLAGS+= -DOPENGL -DNET_ENET -DBSP -DLUA_USE_APICHECK -DTEXTURE_PNG
ifeq ($(DEBUG),1)
CFLAGS+= -DDEBUG_ON -DDEBUG_COMP -DDEBUG_SPOTFX_SOUND -DDEBUG_VIEWPORT
endif

# Include paths
CFLAGS+= -I/usr/include/SDL -I. -Iinclude -Iai/aiinclude -I/usr/include/lua

INCLUDE=$(wildcard include/*.h) $(wildcard ai/aiinclude/*.h)
SRC=$(wildcard *.c) $(wildcard ai/*.c)
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
	$(CC) -o $(BIN) $(LDFLAGS) $(OBJS) $(LIBS)

$(OBJS): $(wildcard include/*.h) $(wildcard ai/aiinclude/*.h)

clean:
	$(RM) $(OBJS) $(BIN)

.PHONY: all clean
