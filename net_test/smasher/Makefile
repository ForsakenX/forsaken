CC=gcc

FLAGS   = -pipe -g -fstack-protector-all
CFLAGS += $(FLAGS) -Wall -Wextra -D_FORTIFY_SOURCE=2
CFLAGS += -DNET_ENET_2 -DDEBUG_ON

CFLAGS += -I. -I../../ `pkg-config --cflags libenet`
LIBS    = `pkg-config --libs libenet`

INCLUDE=../../net.h ../../util.h ../../main.h ../../file.h ../../new3d.h ../../xmem.h
PX_SRC=util.c file.c net_enet_2.c xmem.c
PX_SRC_CPY=$(shell cd ../..; cp $(PX_SRC) net_test/smasher; printf "%s " $(PX_SRC))

SRC=smasher.c $(PX_SRC_CPY)
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=smasher

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)
	$(RM) $(PX_SRC)

.PHONY: all clean
