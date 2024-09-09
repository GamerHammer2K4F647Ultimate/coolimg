**[Features](#Features) |**
**[Plot](#Plot) |**
**[Installing](#Installing) |**
**[Depths](#Depths)**

# coolimg

Simple image viewer.

# Requirments
- git

- libpng, libjpeg, libsdl2, libsdl2-image

- file (unix command)

- glibc (to use musl, you have to rebuild SDL2)

- cc (symlink or hard link to gcc/clang is okay)

- gdb (not required)

# Plot
coolimg is trying to be an app similar to the suckless philosophy.

coolimg is made in SDL2, because "me no know opengl".

# Arguments
-v : version

--help: help

## Running it
```console
$ ./coolimg <image>
```

## Installing

#### Building from source(see [Requirments](#Requirments) for the things you may need)

```console
$ git clone https://github.com/AnonimFan/coolimg.git

$ cd coolimg

$ make -f Makefile.config

$ ./configure

$ sudo make install
```

# Depths

#### Loading images
Done with my implementation:
```c
SDL_Texture*
openIMG(const char* path)
{
    SDL_Texture* img = NULL; // make return valie
    img = IMG_LoadTexture(r, path); // load image
    if (img == NULL)
        printf("\e[0;31mFailed to open image %s: %s\e[0m\n", path, SDL_GetError()); // check if image is still null

    return img; // return the texture
}
```

and with STB_image:
```c
int x, y, n; // declare 3 vars
unsigned char* pixels = stbi_load(argv[1], &x, &y, &n, 0); // load the image argv[1](argument vector index 1)
```

#### Displaying images
Pretty easy with SDL:
```c
void
clear()
{
    SDL_RenderClear(r); // clear the renderer
}

void
render(SDL_Texture* tex, int w, int h)
{
    SDL_Rect src; // declare variable src
    src.x = 0; // set x to 0
    src.y = 0; // set y to 0
    src.w = w; // set w to the width the texture
    src.h = h; // set h to the height the texture

    // same thing
    SDL_Rect dst;
    dst.x = 0;
    dst.y = 0;
    dst.w = w;
    dst.h = h;


    SDL_RenderCopy(r, tex, &src, &dst); // copy the texture, src and dst into r(renderer)
}

void
display()
{
    SDL_RenderPresent(r); // display the renderer
}
```
