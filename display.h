#pragma once

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_SIZE 64*32
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#include "processor.h"



void clearScreen(Chip8State* state);
void drawScreen(Chip8State* state);
void initScreen();