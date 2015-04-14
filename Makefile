CC=g++
CFLAGS= -D_X11 -I/usr/include/ -I./src
LIBS = -lX11

all:
	$(CC) ./src/engine/*.cpp ./src/game/*.cpp $(CFLAGS) -o breakout $(LIBS)

clean:
	rm ./breakout
