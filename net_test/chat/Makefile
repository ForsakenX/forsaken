CC=gcc

FLAGS=-pipe -g -fstack-protector-all
CFLAGS+=$(FLAGS) -Wall -Wextra -D_FORTIFY_SOURCE=2 -DNET_ENET_2 -DDEBUG_ON -I. -I../../ -I/usr/SDL

LIBS=-lenet
INCLUDE=../../net.h ../../util.h ../../main.h ../../file.h ../../new3d.h ../../xmem.h
PX_SRC=util.c file.c net_enet_2.c xmem.c
PX_SRC_CPY=$(shell cd ../..; cp $(PX_SRC) net_test/chat; printf "%s " $(PX_SRC))
SRC=client.c $(PX_SRC_CPY)
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=client

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)
	$(RM) $(PX_SRC)

.PHONY: all clean
