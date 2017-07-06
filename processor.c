typedef struct Chip8State {
    uint8_t     V[16];      //16 8 bit registers
    uint16_t    I;          //Memory address register
    uint16_t    SP;         //Stack pointer     - Starts at 0xEA0
    uint16_t    PC;         //Program counter   - Starts at 0x200
    uint8_t     delay;      //Delay timer
    uint8_t     sound;      //Sound timer
    uint8_t     *memory;
    uint8_t     *screen;    //Display buffer    - Starts at 0xF00 - Display: 64x32
} Chip8State;