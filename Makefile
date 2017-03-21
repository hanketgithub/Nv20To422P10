target  = Nv20To422P10
sources = main.c unpack.c
objects = $(patsubst %.c,%.o,$(sources))
CC = gcc
OPTS = -Wall

all: $(objects)
	$(CC) $(OPTS) -o $(target) $(objects)

clean:
	$(RM) $(target) $(objects)
