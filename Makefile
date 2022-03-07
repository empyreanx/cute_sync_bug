CC     = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wpedantic -I /usr/include/SDL2

#ifeq ($POSIX), 1)
ifeq ($(shell test $(POSIX) -eq 1; echo $$?),0)
    CFLAGS += -DCUTE_SYNC_POSIX
else
    CFLAGS += -DCUTE_SYNC_SDL
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CC = clang
endif

SRCS   = array.c jobs.c utility.c main.c

OBJS   = $(SRCS:.c=.o)

all: tests

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

tests: $(OBJS)
	$(CC) -g -o tests $(OBJS) -lSDL2 -lpthread -lm

.PHONY: clean

clean:
	rm tests *.o
