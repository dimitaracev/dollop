LIBS=-lX11 -lXext -lXrender

SDL=`sdl2-config --cflags --libs`

build:
	gcc main.c -o GameOfLife $(SDL) $(LIBS)

clean:
	rm -f GameOfLife
