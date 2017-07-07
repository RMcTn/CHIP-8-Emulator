#include "display.h"

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Renderer *renderer = NULL;

void printSDLError() {
    printf("Something went wrong with SDL! SDL_ERROR: %s\n", SDL_GetError());
}

void createWindow() {

}

void initDisplay() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printSDLError();
    } else {
        window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printSDLError();
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FreeSurface(screenSurface);

            SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF));
            
            SDL_UpdateWindowSurface(window);
            SDL_Delay(2000);
        }
    }
}