#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void disassemble(uint8_t *codebuffer, int pc) {
    uint8_t *code = &codebuffer[pc];
    uint8_t first_nibble = (code[0] >> 4);
    //code[0] is the first byte
    //code[1] is the second byte
    ///NNN: Address
    //NN: 8-bit constant
    //N: 4-bit constant
    //Vx or Vy is a register
    //I: 16bit register (for memory address)
    printf("%04x %02x %02x ", pc, code[0], code[1]);
    switch(first_nibble) {
        case 0x00: 
            {
                switch(code[1]) {
                    case 0xE0: 
                        {
                            //00E0
                            //Clear the screen;
                            printf("%-10s", "CLS");
                        }
                        break;
                    case 0xEE:
                        {
                            //00EE
                            //Return from a subroutine
                            printf("%-10s", "RTN");
                        }
                }
            }
            break;
        case 0x01:
            {
                //1XNN
                //goto NNN
                printf("%-10s $%01x%02x", "JMP", (code[0] & 0xF), code[1]);
            }
            break;
        case 0x02: 
            {
                //2XNN
                //Call subroutine at NNN
                printf("%-10s $%01x%02x", "CALL", (code[0] & 0xF), code[1]);
            }
            break;
        case 0x03: 
            {
                //3XNN
                //Skips next instruction if Vx == NN
                printf("%-10s V%01x,#$%02x", "SKIP.EQ", (code[0] & 0xF), code[1]);
            }
            break;
        case 0x04: 
            {
                //4XNN
                //Skips next instruction if Vx != NN
                printf("%-10s V%01x,#$%02x", "SKIP.NE", (code[0] & 0xF), code[1]);
            }
            break;
    
        case 0x05: 
            {
                //5XY0
                //Skips next instruction if Vx == Vy
                printf("%-10s V%01x, V%01x", "SKIP.EQ", (code[0] & 0xF), (code[1] >> 4))))))))));
            }
            break;
        case 0x06:
            {
                //6XNN
                //Sets Vx to NN
                printf("%-10s V%01X, #$%02x", "MVI", (code[0] & 0xF), code[1]);
            }
            break;
        case 0x07:
            {
                //7XNN
                //Adds NN to Vx
                printf("%-10s V%01X, #$%02x", "ADD", (code[0] & 0xF), code[1]);
            }
            break;
        case 0x08: 
            {
                switch(code[1] >> 4) {
                    case 0:
                    {
                        //8XY0
                        //Sets Vx to the value of Vy
                        printf("%-10s V%01X, V%01x", "MOV", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x1:
                    {
                        //8XY1
                        //Sets Vx to Vx | Vy
                        printf("%-10s V%01X, V%01x", "OR", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x2:
                    {
                        //8XY2
                        //Sets Vx to Vx & Vy
                        printf("%-10s V%01X, V%01x", "AND", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x3:
                    {
                        //8XY3
                        //Sets Vx to Vx ^ Vy
                        printf("%-10s V%01X, V%01x", "XOR", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x4:
                    {
                        //8XY4
                        //Adds Vy to Vx
                        //Vf set to 1 if carry, 0 if not
                        printf("%-10s V%01X, V%01x", "ADD", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x5:
                    {
                        //8XY5
                        //Subtracts Vy from Vx
                        //Vf set to 0 if there's a borrow, 1 if not
                        printf("%-10s V%01X, V%01x", "SUB", (code[0] & 0XF), code[1] >> 4);
                    }
                    break;
                    case 0x6:
                    {
                        //8XY6
                        //Shifts Vx right by one
                        //Vf set to least significant bit of Vx before shift 
                        printf("%-10s V%01X, V%01x", "ROR", (code[0] & 0XF), code[1] >> 4);
                    }
                    break; 
                    case 0x7:
                    {
                        //8XY7
                        //Sets Vx to Vy minus Vx
                        //Vf set to 0 if there's a borrow, 1 if not
                        printf("%-10s V%01X, V%01x", "SUB", (code[0] & 0XF), code[1] >> 4);
                    }
                    break; 
                    case 0xE:
                    {
                        //8XYE
                        //Shifts Vx left by one
                        //Vf set to most significant bit of Vx before shift 
                        printf("%-10s V%01X, V%01x", "ROL", (code[0] & 0XF), code[1] >> 4);
                    }
                    break; 
                }
                break;
            }
            break;
        case 0x09:
            {
                //9XY0
                //Skips next instruction if Vx doesn't equal Vy
                printf("%-10s V%01x, v%01x", "SKIP.EQ", (code[0] & 0xF), code[1] >> 4);
            }
            break;
        case 0x0a:
            {
            //ANNN
            //Sets I to the address NNN 
            printf("%-10s I, #$%01x%02x", "MVI", code[0] & 0xF, code[1]);
            }
            break;
        case 0x0b: 
            {
                //BNNN
                //Jumps to the address NNN plus V0
                //PC = V0 + NNN
                printf("%-10s, #$%01x%02x", "JMP", code[0] & 0xF, code[1]);
            }
            break;
        case 0x0c: 
            {
                //CXNN
                //Sets Vx to the result of (random number & NN)
                printf("%-10s, V%0x1, #$%0x2", "RNDMSK", code[0] & 0xF, code[1]);
            }
            break;
        case 0x0d: 
            {
                //DXYN
                //Draws a sprite at coord (Vx, Vy)
                //Width of 8 pixels, height of N pixels
                //Vf set if any screen pixels are flipped from set to unset, otherwise set to 0
                printf("%-10s, V%01x, V%01x, #$%01x", "SPRITE", code[0] & 0xF, code[1] >> 4, code[1] & 0xF);
            }
            break;
        case 0x0e: 
            {
                switch(code[1]) {
                    case 0x9E:
                    {
                        //EX9E
                        //Skip next instruction if key stored in Vx is pressed
                        printf("%-10s, V%01x", "SKIPKEY.Y", code[0] & 0xF);
                    }
                    break;
                    case 0xA1:
                    {
                        //EXA1
                        //Skip next instruction if key stored in Vx is NOT pressed
                        printf("%-10s, V%01x", "SKIPKEY.N", code[0] & 0xF);
                    }
                }
                
            }
            break;
        case 0x0f: 
            {
                switch(code[1]) {
                    case 0x07:
                    {
                        //FX07
                        //Sets Vx to the value of the delay timer
                        printf("%-10s, V%01x, DELAY", "MOV", code[0] & 0xF);
                    }
                    break;
                    case 0x0A:
                    {
                        //FX0A
                        //Key press is awaited, then stored in Vx
                        printf("%-10s, V%01x", "KEY", code[0] & 0xF);
                    }
                    break;
                    case 0x15:
                    {
                        //FX15
                        //Sets the delay timer to the value of Vx
                        printf("%-10s, DELAY, V%01x", "MOV", code[0] & 0xF);
                    }
                    break;
                    case 0x18:
                    {
                        //FX18
                        //Sets the sound timer to the value of Vx
                        printf("%-10s, SOUND, V%01x", "MOV", code[0] & 0xF);
                    }
                    break;
                    case 0x1E:
                    {
                        //FX1E
                        //Adds Vx to I
                        printf("%-10s, I, V%01x", "ADI", code[0] & 0xF);
                    }
                    break;
                    case 0x29:
                    {
                        //FX29
                        //Sets I to the location of the sprite for the character in Vx
                        //Characters 0-F are represented by a 4x5 font
                        printf("%-10s, I, V%01x", "SPRITECHAR", code[0] & 0xF);
                    }
                    break;
                    case 0x33:
                    {
                        //FX33
                        //Stores the Binary Coded Decimal representation of Vx,
                        //with the most significant of the three digits at the address in I,
                        //the middle digit at I plus 1,
                        //and the least significant digit at I plus 2.
                        //(Take decimanl of Vx, place hundreds digit in memory at location in I..
                        //tens digit at location I+1, ones digit at location I+2)
                        printf("%-10s, (I), V%01x", "MOVBCD", code[0] & 0xF);
                    }
                    break;
                    case 0x55:
                    {
                        //FX55
                        //Stores V0 to Vx (including Vx) in memory starting at address I
                        printf("%-10s, (I), V0-V%01x", "MOVM", code[0] & 0xF);
                    }
                    break;
                    case 0x65:
                    {
                        //FX65
                        //Fills V0 to Vx (including Vx) with values from memory starting at address I
                        printf("%-10s, V0-V%01x, (I)", "MOVM", code[0] & 0xF);
                    }
                    break;
                }
            }
            break;
    }
}

int main(int argc, char**argv) {
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
    //Read the file into memory at 0x200 and close it
    unsigned char* buffer = malloc(file_size+0x200);

    fread(buffer+0x200, file_size, 1, f);
    fclose(f);

    int pc = 0x200;
    while (pc < (file_size+0x200)) {
        disassemble(buffer, pc);
        //Each instruction is two bytes long
        pc += 2;
        printf("\n");
    }

    free(buffer);
    return 0;
}