/*
 * coolimg
 * Simple Image Viewer
 * Licensed under GPLv2
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include "ansi.h"
#include <string.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Renderer*  r;
SDL_Window*  win;

typedef struct {
    SDL_Texture* img;
    int w, h;
    int x, y;
} Image;

size_t strlen(const char* str);
SDL_Texture* openIMG(const char* path);
void clear();
void render(Image img);
void display();

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// N

SDL_Texture* openIMG(const char* path)
{
    SDL_Texture* img = NULL;
    img = IMG_LoadTexture(r, path);
    if (img == NULL)
        printf("\e[0;31mFailed to open image %s: %s\e[0m\n", path, SDL_GetError());

    return img;
}

void clear()
{
    SDL_RenderClear(r);
}

void render(Image img)
{
    SDL_Rect src;
    src.x = img.x;
    src.y = img.y;
    src.w = img.w;
    src.h = img.h;

    SDL_Rect dst;
    dst.x = img.x;
    dst.y = img.y;
    dst.w = screensizecheck(img.w);
    dst.h = screensizecheck(img.h);


    SDL_RenderCopy(r, img.img, &src, &dst);
}

void display()
{
    SDL_RenderPresent(r);
}

bool VerboseIt = false;

int scaling;

// a = w, b = scr_w
int screensizecheck(int a, int b)
{
    if(a > b) return a / scaling;
}


int
main(int argc, char* argv[])
{
    
    if ( (argc == 2) && !strcmp("-v", argv[1]) || (argc == 2) && !strcmp("--version", argv[1])) {
        printf("\e[0;32mcoolimg 1.3\e[0m\n");
        return 2;
    } else if ( (argc == 2) && !strcmp("--help", argv[1])) {
	fprintf(stdout, "syntax: coolimg [options] [file]");
        fprintf(stdout, "options: \n");
        fprintf(stdout, "   -v, --version : display version\n");
        fprintf(stdout, "   --help : display this\n");
	fprintf(stdout, "   -scalex : scaling (x = multiples of 2 until 8)\n");
        fprintf(stdout, "in-app commands: \n");
        fprintf(stdout, "   i : display image info into terminal window\n");
        fprintf(stdout, "   q : quit\n");
        fflush(stdout);
        fflush(stderr); // vi
        return 0;
    } 
    else if ( (argc == 2) && !strcmp("--about", argv[1])) {
        fprintf(stdout, "\e[0;32mcoolimg 1.2\e[0m\n");
        fprintf(stdout, "-  Depends on: SDL2, SDL2_Image, STB_Image, linux commands\n");
        fprintf(stdout, "Made by coolC\n");
        return 0;
    }
    else if ( (argc == 3) && !strcmp("-scale2", argv[1])) {
	scaling = 2;
    }
    else if ( (argc == 3) && !strcmp("-scale4", argv[1])) {
	scaling = 4;
    }
    else if ( (argc == 3) && !strcmp("-scale6", argv[1])) {
	scaling = 6;
    }
    else if ( (argc == 3) && !strcmp("-scale8", argv[1])) {
	scaling = 8;
    }
    else if (argc < 2) {
	    fprintf(stderr, "\e[1;31merror: \e[4;37mno image provided\e[0m\n");
	    fprintf(stderr, "\e[1;32musage: \e[4;37m%s <image file>\e[0m\n", argv[0]);
    	return 0;
    }

    int x, y, n;
    unsigned char* pixels;
    if(argc == 3) pixels = stbi_load(argv[2], &x, &y, &n, 0);
    else if (argc == 2) pixels = stbi_load(argv[1], &x, &y, &n, 0);
    else printf("argc check failed\n");
    int width = x;
    int height = y;
    fprintf(stdout , "witdh %d \nheight %d\n", x, y);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_TIF);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int scr_w = dm.w;
    int scr_h = dm.h;
    int running = 1;
    char *wind_title = malloc(strlen(argv[1]) * 4);
    sprintf(wind_title, "coolimg %s", argv[1]);
    printf("screen size: %dx%d\n", scr_w, scr_h);
    int w = width, h = height;
    w = screensizecheck(w, scr_w);
    h = screensizecheck(h, scr_h);
    printf("window size: %dx%d\n", w, h);
    win =  SDL_CreateWindow(wind_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        return -1;
    }

    r =  SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (r  == NULL) return -1;
    
    Image image;

    if(argc > 1)
    {
        image.x = 0;
        image.y = 0;

        image.w = w;
        image.h = h;
	if(argc == 3) image.img = openIMG(argv[2]);
	else if (argc == 2) image.img = openIMG(argv[1]);
	else printf("argc check failed\n");
        int speed = 300;
        SDL_Event event;
        while (running == 1)
        {
            while (SDL_PollEvent(&event))
            {
                switch(event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                        case SDLK_q:
                            fprintf(stdout, "Exiting...\n");
                            image.img = NULL;
                            r = NULL;
                            running = 0;
                            break;
                        case SDLK_i:
                            fprintf(stdout, "image info: ");
                            fflush(stdout);
                            char* command = NULL;
                            int argvl = strlen(argv[1]);
                            command = malloc( 9 + argvl + 78 + 1);
			    strcpy(command, "file --brief ");
                            strcat(command, argv[1]);
                            system(command);
                            printf("\n");
                            break;
                      //  case SDLK_UP:
                      //      image.w += 1;
                      //      image.h += 1;
                      //  case SDLK_DOWN:
                      //      image.w -= 1;
                      //      image.h -= 1;
                        }


                }
            }
            clear();
            render(image);
            display();
        }
    }



    SDL_DestroyRenderer(r);
    IMG_Quit();
    SDL_DestroyWindow(win);
    return 0;
}

