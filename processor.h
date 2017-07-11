#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define FONT_BASE 0
#define FONT_SIZE 5*16  //5 bytes each * 16 characters

typedef struct Chip8State {
    uint8_t     V[16];      //16 8 bit registers
    uint16_t    I;          //Memory address register
    uint16_t    SP;         //Stack pointer     - Starts at 0xEA0
    uint16_t    PC;         //Program counter   - Starts at 0x200
    uint8_t     delay;      //Delay timer
    uint8_t     sound;      //Sound timer
    uint8_t     *memory;    //4k RAM
    uint8_t     *screen;    //Display buffer    - Starts at 0xF00 - Display: 64x32
    uint8_t     key_state[16];
    bool        waiting_for_key;
    uint8_t     save_key_state[16];
    bool        draw_flag;
} Chip8State;

