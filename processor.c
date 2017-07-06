#include "processor.h"

typedef struct Chip8State {
    uint8_t     V[16];      //16 8 bit registers
    uint16_t    I;          //Memory address register
    uint16_t    SP;         //Stack pointer     - Starts at 0xEA0
    uint16_t    PC;         //Program counter   - Starts at 0x200
    uint8_t     delay;      //Delay timer
    uint8_t     sound;      //Sound timer
    uint8_t     *memory;    //4k RAM
    uint8_t     *screen;    //Display buffer    - Starts at 0xF00 - Display: 64x32
} Chip8State;

Chip8State* init() {
    Chip8State* state = calloc(sizeof(Chip8State), 1);
    
    //Change these numbers to variables in the future
    //1024*4 for 4k
    state->memory = calloc(1024*4, 1);
    state->screen = &state->memory[0xF00];
    state->SP = 0xFA0;
    state->PC = 0x200;

    return state;
}

void emulate(Chip8State* state) {
    uint8_t* opcode = &state->memory[state->PC];
    int first_nibble = (opcode[0] & 0xF0) >> 4;
    printf("%d\n", first_nibble);
    printf("%d\n", *opcode >> 4);
    switch(first_nibble)
    {
        case 0x00: uninplementedInstruction(state); break;
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
            uint8_t reg = getRegister(opcode[0]);
            if(state->V[reg] == opcode[1]) {
                //Skip next instruction
                incrementPC(); 
            }
            incrementPC();
        }
        break;
        case 0x04:
        {
            //SKIP.NE Vx NN
            uint8_t reg = getRegister(opcode[0]);
            if (state->V[reg] != opcode[1]) {
                //Skip next instruction
                incrementPC();
            }
            incrementPC();
        }
        break;
        case 0x05:
        {
            //SKIP.EQ Vx Vy
            uint8_t first_reg = getRegister(opcode[0]);
            uint8_t second_reg = getRegister(opcode[1]);
            if (state->V[first_reg] == state->V[second_reg]) {
                //Skip next instruction
                incrementPC();
            }
            incrementPC();
        }
        break;
        case 0x06:
        {
            //MVI Vx NN
            uint8_t reg = getRegister(opcode[0]);
            state->V[reg] = opcode[1];
            incrementPC();
        }
        break;
        case 0x07:
        {
            //ADDI Vx NN
            uint8_t reg = getRegister(opcode[0]);
            state->V[reg] += opcode[1];
            incrementPC();
        }
    }
}

void uninplementedInstruction(Chip8State* state) {

}

int main() {
    Chip8State* state = init();
    emulate(state);
}

void incrementPC(Chip8State* state) {
    state->[PC] += 2;
}

uint16_t getAddress(uint8_t first_byte, uint8_t second_byte) {
    //XN
    first_byte = first_byte & 0x0F;
    //Return format of XNNN
    return (first_byte << 8) | second_byte;
}

uint8_t getRegister(uint8_t first_byte) {
    return first_byte & 0x0F;
}