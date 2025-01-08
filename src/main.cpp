#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "log.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

SDL_Renderer* r;
SDL_Window* win;
int scale;

typedef struct {
	SDL_Texture* img;
	int w, h; // size
	int x, y; // pos
} Image;

SDL_Texture* openIMG(const char* path)
{
	SDL_Texture* img = NULL;
	img = IMG_LoadTexture(r, path);
	if (img == NULL) {
		std::cout << "failed to open image, " << SDL_GetError() << std::endl;
	}
	return img;
}

void clear()
{
	SDL_RenderClear(r);
}

void render(Image img)
{
	SDL_Rect src;
	SDL_Rect dst;
	src.x = img.x;
	src.y = img.y;
	src.w = img.w;
	src.h = img.h;
	dst.x = img.x;
	dst.y = img.y;
	dst.w = img.w / scale;
	dst.h = img.h / scale;
	
	SDL_RenderCopy(r, img.img, &src, &dst);
}

void display()
{
	SDL_RenderPresent(r);
}

bool isImageFile(const std::string& filename) 
{
    const std::vector<std::string> imageExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".webp", ".img"};
    size_t dotPosition = filename.find_last_of('.');
    if (dotPosition == std::string::npos) {
        return false; // No extension found
    }
    std::string extension = filename.substr(dotPosition);
    for (char& ch : extension) {
        ch = std::tolower(ch);
    }
    for (const std::string& ext : imageExtensions) {
        if (extension == ext) {
            return true;
        }
    }
    return false;
}

bool should_be_scaled = false;

int main(int argc, char *argv[])
{
	if(argc == 1) 
	{
		logerr(stdout, "no arguments given");
		loginf(stdout, "use --help for help");
		return 1;
	}
	
	scale = 1;     
	std::string filename;  
	for (int i = 1; i < argc; ++i) {
       		std::string arg = argv[i];

        	if (arg == "-scale" && i + 1 < argc) {
                	scale = std::stoi(argv[++i]);
        	} else if (isImageFile(arg)) {	
            		filename = arg;
        	}
    	}

	const char* fn = filename.c_str();

	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		logerr(stdout, SDL_GetError());
	}
	if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP)) {
		logerr(stdout, SDL_GetError());
	}

	char* title = (char*)malloc(256 + strlen(argv[0]));
	sprintf(title, "coolimg %s", fn);
	int x, y, n; // x = width, y = height
	unsigned char* pixels = stbi_load(fn, &x, &y, &n, 0);
	std::cout << "res: " << x << "x" << y << std::endl; // eg.: 1920x1080

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	int scr_w = dm.w;
	int scr_h = dm.h;
	std::cout << "screen size: " << scr_w << "x" << scr_h << std::endl;

	win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x / scale, y / scale, SDL_WINDOW_SHOWN);
	if(win == NULL) std::cout << "failed to create window: " << SDL_GetError() << std::endl;

	r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if(r == NULL) std::cout << "failed to create renderer: " << SDL_GetError() << std::endl;

	Image image;

	if (argc > 1) {
		image.x = 0;
		image.y = 0;
		image.w = x; // x, as in value got from stbi_load()
		image.h = y; // y, as in value got from stbi_load()
	
		image.img = openIMG(fn);
		int speed = 300; // what?
		SDL_Event e;
		bool running = true;
		while(running) {
			while (SDL_PollEvent(&e)) {
				switch(e.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
					case SDLK_q:
						std::cout << "exiting..." << std::endl;
						image.img = NULL;
						running = false;
					}
				}
			}
			clear();
			render(image);
			display();
		}
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
	free(title);

}
