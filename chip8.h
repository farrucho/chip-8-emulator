#ifndef CHIP_8_H
#define CHIP_8_H

#include <cstdint>

class Chip8 {
    /* 
    http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
    https://austinmorlan.com/posts/chip8_emulator/
    https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
    https://github.com/kripod/chip8-roms
    https://github.com/Timendus/chip8-test-suite
    */
    // NAO SEI SE TA CERTO FUNCAO SHR_Vx_Vy()


    private:
        uint8_t memory_map[4096]; 
        
        // Vx registers, where x is a hexadecimal digit (0 through F)
        // The VF register should not be used by any program, as it is used as a flag by some instructions
        uint8_t V[16];

        // This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
        uint16_t I;  

        // When these registers are non-zero, they are automatically decremented at a rate of 60Hz. See the section 2.5, Timers & Sound, for more information on these.
        uint8_t delay_timer;
        uint8_t sound_timer;


        // The program counter (PC) should be 16-bit, and is used to store the currently executing address in the 12 most right bits.
        // Keep in mind every instruction is 2 bytes long and address only store 8 bits (1 byte)
        // So this value will get incremented or decremented in 2
        uint16_t pc;
        
        // Used to store the address that the interpreter shoud return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
        // https://www.geeksforgeeks.org/subroutine-subroutine-nesting-and-stack-memory/
        // In a stack, the first data put will be the last to get out of a stack.
        uint16_t stack[16];
        // The stack pointer (SP) can be 8-bit, it is used to point to the topmost level of the stack. It Always points to the next available (empty) position on the stack, not the last valid entry.
        uint8_t sp;
        
        // opcode (operation code) is an enumerated value that specificies the operation to be performed
        uint16_t opcode;

    public:
        uint8_t keyboard[16];

        // 64 pixels wide and 32 pixels high, each pixel is either on or off, so only two colors can be represented.
        uint8_t display[64 * 32];

        void initialize();
        void loadRom(char const* filename);

        // nnn or addr -> A 12-bit value, the lowest 12 bits of the instruction
        // n or nibble -> A 4-bit value, the lowest 4 bits of the instruction
        // x -> A 4-bit value, the lower 4 bits of the high byte of the instruction
        // y -> A 4-bit value, the upper 4 bits of the low byte of the instruction
        // kk or byte -> An 8-bit value, the lowest 8 bits of the instruction

        void SYS_addr();
        void CLS();
        void RET();
        void JP_addr();
        void CALL_addr();
        void SE_Vx_byte();
        void SNE_Vx_byte();
        void SE_Vx_Vy();
        void LD_Vx_byte();
        void ADD_Vx_byte();
        void LD_Vx_Vy();
        void OR_Vx_Vy();
        void AND_Vx_Vy();
        void XOR_Vx_Vy();
        void ADD_Vx_Vy();
        void SUB_Vx_Vy();
        void SHR_Vx_Vy();
        void SUBN_Vx_Vy();
        void SHL_Vx_Vy();
        void SNE_Vx_Vy();
        void LD_I_addr();
        void JP_V0_addr();
        void RND_Vx_byte();
        void DRW_Vx_Vy_nibble();
        void SKP_Vx();
        void SKNP_Vx();
        void LD_Vx_DT();
        void LD_Vx_K();
        void LD_DT_Vx();
        void LD_ST_Vx();
        void ADD_I_Vx();
        void LD_F_Vx();
        void LD_B_Vx();
        void LD_I_Vx();
        void LD_Vx_I();
};




#endif