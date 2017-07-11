#include "processor.h"
#include "display.h"
#include "disassembler.c"

void uninplementedInstruction(Chip8State* state);
void incrementPC(Chip8State* state);
uint16_t getAddress(uint8_t first_byte, uint8_t second_byte);
uint8_t getRegister(uint8_t byte, bool first_nibble);

uint8_t font4x5[] = {
//0
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,

//1
    0b01100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b01110000,

//2
    0b11100000,
    0b00010000,
    0b00110000,
    0b01100000,
    0b11110000,
//3
    0b11100000,
    0b00010000,
    0b01100000,
    0b00010000,
    0b11100000,

    0b10100000,
    0b10100000,
    0b11100000,
    0b00100000,
    0b00100000,

    0b11110000,
    0b10000000,
    0b11110000,
    0b00010000,
    0b11110000,

    0b10000000,
    0b10000000,
    0b11110000,
    0b10010000,
    0b11110000,

    0b11110000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b00100000,

    0b11110000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b11110000,

    0b11110000,
    0b10010000,
    0b11110000,
    0b00010000,
    0b00010000,

    0b01100000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,

    0b10000000,
    0b10000000,
    0b11110000,
    0b10010000,
    0b11110000,

    0b11110000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11110000,

    0b11100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11100000,

    0b11110000,
    0b10000000,
    0b11100000,
    0b10000000,
    0b11110000,

    0b11110000,
    0b10000000,
    0b11100000,
    0b10000000,
    0b10000000,
};
Chip8State* init() {
    srand(time(NULL));
    Chip8State* state = calloc(sizeof(Chip8State), 1);
    
    //Change these numbers to variables in the future
    //1024*4 for 4k
    state->memory = calloc(1024*4, 1);
    state->screen = &state->memory[0xF00];
    
    state->SP = 0xFA0;
    state->PC = 0x200;

    //Put font into low memory
    memcpy(&state->memory[FONT_BASE], font4x5, FONT_SIZE);
    initScreen();

    return state;
}

void emulate(Chip8State* state) {
    uint8_t* opcode = &state->memory[state->PC];
    int first_nibble = (opcode[0] & 0xF0) >> 4;
    // printf("%d\n", first_nibble);
    // printf("%d\n", *opcode >> 4);
    if (state->draw_flag) {
        drawScreen(state);
        } else {
    switch(first_nibble)
    {
        case 0x00:
        {
            printf("Clearing screen");
            clearScreen(state);
            
            incrementPC(state);
        }
        break;
        case 0x01:
        {
            //JMP NNN
            uint16_t target = getAddress(opcode[0], opcode[1]); 
            state->PC = target;
        }
        break;
        case 0x02: uninplementedInstruction(state); break;
        case 0x03:
        {
            //SKIP.EQ Vx NN
            uint8_t reg = getRegister(opcode[0], false);
            if(state->V[reg] == opcode[1]) {
                //Skip next instruction
                incrementPC(state); 
            }
            incrementPC(state);
        }
        break;
        case 0x04:
        {
            //SKIP.NE Vx NN
            uint8_t reg = getRegister(opcode[0], false);
            if (state->V[reg] != opcode[1]) {
                //Skip next instruction
                incrementPC(state);
            }
            incrementPC(state);
        }
        break;
        case 0x05:
        {
            //SKIP.EQ Vx Vy
            uint8_t first_reg = getRegister(opcode[0], false);
            uint8_t second_reg = getRegister(opcode[1], true);
            if (state->V[first_reg] == state->V[second_reg]) {
                //Skip next instruction
                incrementPC(state);
            }
            incrementPC(state);
        }
        break;
        case 0x06:
        {
            //MVI Vx NN
            uint8_t reg = getRegister(opcode[0], false);
            state->V[reg] = opcode[1];
            incrementPC(state);
        }
        break;
        case 0x07:
        {
            //ADDI Vx NN
            uint8_t reg = getRegister(opcode[0], false);
            state->V[reg] += opcode[1];
            incrementPC(state);
        }
        break;
        case 0x08:
        {
            switch (opcode[1] & 0x0F) {
                case 0x0:
                {
                    //MOV Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    state->V[first_reg] = state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x1:
                {
                    //OR Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    state->V[first_reg] = state->V[first_reg] | state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x2:
                {
                    //AND Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    state->V[first_reg] = state->V[first_reg] & state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x3:
                {
                    //XOR Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    state->V[first_reg] = state->V[first_reg] ^ state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x4:
                {
                    //ADD Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    //Check if VF needs to be set (carry)
                    if(state->V[first_reg] + state->V[second_reg] > 255) {
                        state->V[0xF] = 1;
                    } else {
                        state->V[0xF] = 0;
                    }
                    state->V[first_reg] += state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x5:
                {
                    //SUB Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    //Check if VF needs to be set (borrow)
                    if (state->V[first_reg] > state->V[second_reg]) {
                        state->V[0xF] = 1;
                    } else {
                        state->V[0xF] = 0;
                    }
                    state->V[first_reg] -= state->V[second_reg];
                    incrementPC(state);
                }
                break;
                case 0x6:
                {
                    //ROR Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    //Set VF to LSB BEFORE shift
                    state->V[0xF] = state->V[reg] & 0x1;
                    state->V[reg] = state->V[reg] >> 1;
                    incrementPC(state);
                }
                break;
                case 0x7:
                {
                    //SUBB Vx Vy
                    uint8_t first_reg = getRegister(opcode[0], false);
                    uint8_t second_reg = getRegister(opcode[1], true);
                    //Check if VF needs to be set (borrow)
                    if (state->V[first_reg] < state->V[second_reg]) {
                        state->V[0xF] = 1;
                    } else {
                        state->V[0xF] = 0;
                    }
                    state->V[first_reg] = state->V[second_reg] - state->V[first_reg];
                    incrementPC(state);
                }
                break;
                case 0xE:
                {
                    //ROL Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    //Set VF to MSB BEFORE shift
                    state->V[0xF] = (0x80 == (state->V[reg] & 0x80));
                    state->V[reg] = state->V[reg] << 1;
                    incrementPC(state);
                }
                break;
            }
            break;
        }
        break;
        case 0x09:
        {
            //SKIP.NE Vx Vy
            uint8_t first_reg = getRegister(opcode[0], false);
            uint8_t second_reg = getRegister(opcode[1], true);
            if (state->V[first_reg] != state->V[second_reg]) {
                //Skip next instruction
                incrementPC(state);
            }
            incrementPC(state);
        }
        break;
        case 0x0A:
        {
            //MVI I NNN
            uint16_t address = getAddress(opcode[0], opcode[1]);
            state->I = address;
            incrementPC(state);
        }
        break;
        case 0x0B:
        {
            //JMP NNN + V0
            uint16_t address = getAddress(opcode[0], opcode[1]);
            address += state->V[0];
            state->PC = address;
        }
        break;
        case 0x0C:
        {
            //RNDMSK Vx NN
            uint8_t reg = getRegister(opcode[0], false);
            int random = rand() % 255;
            state->V[reg] = random & opcode[1];
            incrementPC(state);
        }
        break;
        case 0x0D:
        {
            //Draw sprite at (vx, vy)
            //SPRITE
            int lines = opcode[1] & 0xF;
            int x = state->V[opcode[0] & 0xF];
            int y = state->V[(opcode[1] & 0xF0) >> 4];

            state->draw_flag = true;
            printf("drawflag set\n");
            state->V[0xF] = 0;
            for (int i = 0; i < lines; i++) {
                uint8_t *sprite = &state->memory[state->I+i];
                int spritebit=7;
                for (int j=x; j<(x+8) && j<64; j++) {
                    int jover8 = j / 8; //Picks the byte in the row
                    int jmod8 = j % 8; //Picks the bit in the byte
                    uint8_t srcbit = (*sprite >> spritebit) & 0x1;

                    if (srcbit) {
                        uint8_t *destbyte_p = &state->screen[(i+y) * (64/8) + jover8];
                        uint8_t destbyte = *destbyte_p;
                        uint8_t destmask = (0x80 >> jmod8);
                        uint8_t destbit = destbyte & destmask;

                        srcbit = srcbit << (7-jmod8);

                        if (srcbit & destbit) {
                            state->V[0xF] = 1;

                        }

                        destbit ^= srcbit;

                        destbyte = (destbyte & ~destmask) | destbit;

                        *destbyte_p = destbyte;
                    }
                    spritebit--;
                }
            }
            drawScreen(state);
            incrementPC(state);
        }
        break;
        case 0x0E:
        {
            switch(opcode[1])
            {
                case 0x9E:
                {
                    //SKIPKEY.Y Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    if (state->key_state[state->V[reg]] != 0) {
                        //Skip next instruction
                        incrementPC(state);
                    }
                    incrementPC(state);
                }
                break;
                case 0xA1:
                {
                    //SKIPKEY.N
                    uint8_t reg = getRegister(opcode[0], false);
                    if (state->key_state[state->V[reg]] == 0) {
                        //Skip next instruction
                        incrementPC(state);
                    }
                    incrementPC(state);
                }
            }
            break;
        }
        break;
        case 0x0F: 
        {
            switch(opcode[1]) {
                case 0x07:
                {
                    //MOV Vx DELAY
                    uint8_t reg = getRegister(opcode[0], false);
                    state->V[reg] = state->delay;
                    incrementPC(state);
                }
                break;
                case 0x0A:
                {
                    //KEY Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    if (state->waiting_for_key == 0) {
                        //Start waiting for a key
                        memcpy(&state->save_key_state, &state->key_state, 16);
                        return;
                    } else {
                        //Look for a change in key_state
                        for (int i = 0; i < 16; i++) {
                            if ((state->save_key_state[i] == 0) && state->key_state[i] == 1) {
                                state->waiting_for_key = 0;
                                state->V[reg] = i;
                                incrementPC(state);
                                return;
                            }
                            //copy key state to detect key press,release then press
                            state->save_key_state[i] = state->key_state[i];
                        }
                        return;
                    }
                }
                break;
                case 0x15:
                {
                    //MOV DELAY Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    state->delay = state->V[reg];
                    incrementPC(state);
                }
                break;
                case 0x18:
                {
                    //MOV SOUND Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    state->sound = state->V[reg];
                    incrementPC(state);
                }
                break;
                case 0x1E:
                {
                    //ADI I Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    state->I += state->V[reg];
                    incrementPC(state);
                }
                break;
                case 0x29:
                {
                    //SPRITECHAR I Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    state->I = FONT_BASE + (state->V[reg] * 5);
                }
                break;
                case 0x33:
                {
                    //MOVBCD (I) Vx
                    uint8_t ones, tens, hundreds;
                    uint8_t reg = getRegister(opcode[0], false);
                    uint8_t value = state->V[reg];
                    ones = value % 10;
                    value = value / 10;
                    tens = value % 10;
                    hundreds = value / 10;
                    state->memory[state->I] = hundreds;
                    state->memory[state->I+1] = tens;
                    state->memory[state->I+2] = ones;
                    incrementPC(state);
                }
                break;
                case 0x55:
                {
                    //MOVM (I) V0->Vx
                    uint8_t reg = getRegister(opcode[0], false);
                    uint16_t address = state->I;
                    for (uint8_t current_reg = 0; current_reg <= reg; current_reg++) {
                        state->memory[address + current_reg] = state->V[current_reg];
                    }
                    incrementPC(state);
                }
                break;
                case 0x65:
                {
                    //MOVM V0->Vx (I)
                    uint8_t reg = getRegister(opcode[0], false);
                    uint16_t address = state->I;
                    for (uint8_t current_reg = 0; current_reg <= reg; current_reg++) {
                        state->V[current_reg] = state->memory[address + current_reg];
                    }
                    incrementPC(state);
                }
                break;
            }
            break;
        }
        break;
    }
    }
}

void uninplementedInstruction(Chip8State* state) {

}

int main(int argc, char* argv[]) {
    Chip8State* state = init();
    FILE *f = fopen(argv[1], "rb");
    if (f==NULL) {
        printf("Error: Couldn't open %s\n", argv[1]);
        exit(1);
    }

    //Get file size
    fseek(f, 0L, SEEK_END);
    int file_size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    //CHIP-8 puts programs in memory at 0x200
    //Read the file int/o memory at 0x200 and close it
    unsigned char* buffer = malloc(file_size+0x200);

    fread(buffer+0x200, file_size, 1, f);
    // fread(&state->memory[state->PC], file_size, 1, f);
    fclose(f);

    // drawScreen(state);
    memcpy(&state->memory[state->PC], buffer + 0x200, file_size);
    while (state->PC < (file_size+0x200)) {
        disassemble(buffer, state->PC);
        emulate(state);
        //Each instruction is two bytes long
        printf("\n");
    }

    free(buffer);

    // emulate(state);
    

    /*
     * WHITE
     * BOARD
     * THIS
     * SHIT
     * !!!
     */
            //Draw sprite at (vx, vy)
            //SPRITE
            // int lines = 5;
            // int x = 0;
            // int y = 0;

            // state->draw_flag = true;
            // printf("drawflag set\n");
            // state->V[0xF] = 0;
            // for (int i = 0; i < lines; i++) {
            //     uint8_t *sprite = &state->memory[state->I+i];
            //     int spritebit=7;
            //     for (int j=x; j<(x+8) && j<64; j++) {
            //         int jover8 = j / 8; //Picks the byte in the row
            //         int jmod8 = j % 8; //Picks the bit in the byte
            //         uint8_t srcbit = (*sprite >> spritebit) & 0x1;

            //         if (srcbit) {
            //             uint8_t *destbyte_p = &state->screen[(i+y) * (64/8) + jover8];
            //             uint8_t destbyte = *destbyte_p;
            //             uint8_t destmask = (0x80 >> jmod8);
            //             uint8_t destbit = destbyte & destmask;

            //             srcbit = srcbit << (7-jmod8);

            //             if (srcbit & destbit) {
            //                 state->V[0xF] = 1;

            //             }

            //             destbit ^= srcbit;

            //             destbyte = (destbyte & ~destmask) | destbit;

            //             *destbyte_p = destbyte;
            //         }
            //         spritebit--;
            //     }
            // }
            
    // state->screen[0] = 0b1111;
    // state->screen[1] = 255;
    // state->screen[155]= 255;
    // state->screen[32] = 255;
    // state->screen[255] = 255;
    // state->screen[64] = 1;
    // state->screen[63] = 1;



    state->draw_flag = true;
    drawScreen(state);
    return 0;
}

void incrementPC(Chip8State* state) {
    state->PC += 2;
}

uint16_t getAddress(uint8_t first_byte, uint8_t second_byte) {
    //XN
    first_byte = first_byte & 0x0F;
    //Return format of XNNN
    return (first_byte << 8) | second_byte;
}

uint8_t getRegister(uint8_t byte, bool first_nibble) {
    if (first_nibble)
        return byte & 0xF0;
    else
        return byte & 0x0F;
}
