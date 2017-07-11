#include "display.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
uint8_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT * 4];


void clearScreen(Chip8State* state) {
    memset(state->screen, 0,SCREEN_SIZE/8);
}

void drawScreen(Chip8State* state)
{
    printf("DRAWING\n");
    if (state->draw_flag)
    {
        //each bit in the byte at state->screen[i] represents a pixel!!!
        //check whiteboard for possible algorithm
        for (int i = 0; i < SCREEN_SIZE / 8; i++)
        {
            int x = i % SCREEN_WIDTH;
            int y = i / SCREEN_WIDTH;
            printf("X: %d\nY: %d\nI: %d\n", x, y, i);
            uint8_t temp_byte = state->screen[i];
            for (int j = 0; j < 8; j++)
            {
                unsigned int offset; 
                // unsigned int offset = (SCREEN_WIDTH * y * 4) + (x * 4) + (j* 4);
                offset = i * 32 + (j * 4);

                printf("Offset: %d\n", offset);
                if (temp_byte & 0x80)
                {
                    //Turn pixel white (on)
                    pixels[offset + 0] = 255;
                    pixels[offset + 1] = 255;
                    pixels[offset + 2] = 255;
                    pixels[offset + 3] = SDL_ALPHA_OPAQUE;
                } else {
                    //Turn pixel black (off)
                    pixels[offset + 0] = 0;
                    pixels[offset + 1] = 0;
                    pixels[offset + 2] = 0;
                    pixels[offset + 3] = SDL_ALPHA_OPAQUE;
                }
                temp_byte = temp_byte << 1;
            }
            // if (state->screen[i] == 0)
            // {
            //     pixels[offset + 0] = 0;
            //     pixels[offset + 1] = 0;
            //     pixels[offset + 2] = 0;
            //     pixels[offset + 3] = SDL_ALPHA_OPAQUE;
            // }
            // else
            // {
            //     pixels[offset + 0] = 255;
            //     pixels[offset + 1] = 255;
            //     pixels[offset + 2] = 255;
            //     pixels[offset + 3] = SDL_ALPHA_OPAQUE;
            // }
        }
        SDL_UpdateTexture(texture, NULL, &pixels[0], SCREEN_WIDTH * 4);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        state->draw_flag = false;
        printf("NO LONGER DRAWING\n");
        // SDL_Delay(300);
    }
}

void initScreen() {
    
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    printf("Render name: %s\n", info.name);
    printf("Texture formats: ");
    for (int i = 0; i < info.num_texture_formats; i++) {
        printf("%d\n", info.texture_formats[i]);
    }
}


        // while( SDL_PollEvent( &event ) )
        // {
        //     if( ( SDL_QUIT == event.type ) ||
        //         ( SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode ) )
        //     {
        //         running = false;
        //         break;
        //     }
        // }