include Makefile.config

all: build

build:
	$(CC) src/*.cpp -o coolimg -lm -Iinclude -lSDL2 -lSDL2_image -lSDL2main -Wall

clean: 
	rm -rf *.o
	rm -rf src/*.o
	rm -rf ./coolimg
