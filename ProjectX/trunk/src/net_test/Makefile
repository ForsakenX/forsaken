CC=gcc

FLAGS=-pipe -g -fstack-protector-all
CFLAGS+=$(FLAGS) -Wall -Wextra -D_FORTIFY_SOURCE=2 -DNET_ENET_2 -DDEBUG_ON -I. -I../ -I../include -I/usr/include/SDL

LIBS=-lenet
INCLUDE=../net.h ../util.h ../render.h ../include/main.h ../include/file.h ../include/new3d.h ../include/xmem.h
SRC=client.c ../util.c ../file.c ../net_enet_2.c ../xmem.c
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=client

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)

.PHONY: all clean
