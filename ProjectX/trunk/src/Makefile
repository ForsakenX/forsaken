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

FLAGS=-pipe
CFLAGS=$(FLAGS)
LDFLAGS=$(FLAGS)

ifeq ($(DEBUG),1)
	FLAGS+= -g 
	CFLAGS+=-Wall -Wextra 
else
	CFLAGS+=-O2
	LDFLAGS+=-s
endif

ifeq ($(SSP),1)
	FLAGS+= -fstack-protector-all
	CFLAGS+= -D_FORTIFY_SOURCE=2
endif

ifeq ($(MINGW),1)

CFLAGS += -DMINGW `sdl-config --cflags`
LIBS += -lm -lglu32 -lopengl32 -lenet `sdl-config --libs`
LIBS += -llua -lsocket -lmime -lluasocket -lws2_32 -lwsock32 -lpng14 -lz -lwinmm -lOpenAL32 

else

CFLAGS+= -I/usr/include/SDL -I/usr/include/lua -I/usr/include/AL

ifeq ($(STATIC),1)
	LIBS=-lSDL -lGL -lGLU -Wl,-dn -lenet -llua -llua-mime -llua-socket -Wl,-dy -lm -lpng12 -lz -lopenal
else
	LIBS=-lSDL -lGL -lGLU -lenet -llua -llua-mime -llua-socket -lm -lpng12 -lz -lopenal
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

ifeq ($(GL),3)
	CFLAGS+= -DOPENGL3
else
	CFLAGS+= -DOPENGL1
endif

# ProjectX-specific includes
CFLAGS += -I. -Iinclude -Iai/aiinclude

# ProjectX-specific macros
CFLAGS+= -DOPENGL -DNET_ENET_2 -DBSP -DLUA_USE_APICHECK -DTEXTURE_PNG -DSOUND_SUPPORT -DSOUND_OPENAL
ifeq ($(DEBUG),1)
	CFLAGS+= -DDEBUG_ON -DDEBUG_COMP -DDEBUG_SPOTFX_SOUND -DDEBUG_VIEWPORT 
endif

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
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(wildcard include/*.h) $(wildcard ai/aiinclude/*.h)

clean:
	$(RM) $(OBJS) $(BIN)

.PHONY: all clean
