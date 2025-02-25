#ifndef CHIP_8_H
#define CHIP_8_H

#include <cstdint>

class Chip8 {
    /* 
    http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
    https://austinmorlan.com/posts/chip8_emulator/
    */

    private:
        uint8_t memory_map[4096]; 
        
        uint8_t V[16];
        // Vx registers, where x is a hexadecimal digit (0 through F)
        // The VF register should not be used by any program, as it is used as a flag by some instructions

        uint16_t I;  // This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.

        uint8_t delay_timer;
        uint8_t sound_timer;
        // When these registers are non-zero, they are automatically decremented at a rate of 60Hz. See the section 2.5, Timers & Sound, for more information on these.


        uint16_t PC;
        uint8_t SP;
        // The program counter (PC) should be 16-bit, and is used to store the currently executing address. The stack pointer (SP) can be 8-bit, it is used to point to the topmost level of the stack.

        uint16_t stack[16];
        // Used to store the address that the interpreter shoud return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.

    public:
        uint8_t keyboard[16];

        uint8_t display[64 * 32];
        // 64 pixels wide and 32 pixels high, each pixel is either on or off, so only two colors can be represented.

        void initialize();
};




#endif