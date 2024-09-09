include Makefile.config

all: build

man:
	pandoc man/source/coolimg.1.md -s -t man -o ./coolimg.1
	sudo mkdir -pv /usr/local/man/man1
	gzip ./coolimg.1
	sudo cp ./coolimg.1 /usr/local/man/man1
	sudo mandb

imginfo:
	$(CC) $(CFLAGS) -c src/imginfo.c -o src/imginfo.o
	$(CC) src/imginfo.o -o ./imginfo -lm -lSDL2 -lSDL2main -Wall

build:
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o
	$(CC) src/main.o -o ./coolimg -lm -lSDL2 -lSDL2_image -lSDL2main -Wall
	@printf "building done by $(USER)\n"

install:
	cp ./coolimg /usr/bin

uninstall:
	rm -rf /usr/bin/coolimg

remove: uninstall

clean: uninstall
	rm -rf src/main.o
	rm -rf ./coolimg
