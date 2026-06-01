CC = clang
CFLAGS = -Wall -std=c99
RAYLIB = $(shell brew --prefix raylib)

INCLUDES = -I$(RAYLIB)/include
LIBS = -L$(RAYLIB)/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC = main.c
OUT = game

all:
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) $(LIBS) -o $(OUT)

clean:
	rm -f $(OUT)