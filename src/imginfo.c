#include <stdio.h>
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
	printf("anyádért nem adtál meg egy képet te balfasz");
	return 1;
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int scr_w, scr_h;
    scr_w = dm.w;
    scr_h = dm.h;
    int x, y, n;
    unsigned char* pixels = stbi_load(argv[1], &x, &y, &n, 0);
    printf("image size: %dx%d\n", x, y); 
    fprintf(stdout, "image info: ");
    fflush(stdout);
    char* command = NULL;
    int argvl = strlen(argv[1]);
    command = malloc( 9 + argvl + 78 + 1);
    strcpy(command, "file --brief ");
    strcat(command, argv[1]);
    system(command);
    printf("\n");
    stbi_image_free(pixels);
    SDL_Quit();
    free(command);
    return 0;
}
