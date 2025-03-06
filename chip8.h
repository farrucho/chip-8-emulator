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

    public:
        uint8_t keyboard[16];

        // 64 pixels wide and 32 pixels high, each pixel is either on or off, so only two colors can be represented.
        uint8_t display[64*32] = {};

        void initialize();
        void loadRom(char const *filename);
        void Cycle();
        uint8_t memory_map[4096] = {}; 

        uint16_t getOpcode();
        uint16_t getOpcodeFunctionPtr();
        uint8_t getV(uint8_t Vindex);

    private:
        // All instructions are 2 bytes long and are stored most-significant-byte first. 
        // In memory, the first byte of each instruction should be located at an even addresses. If a program includes sprite data, it should be padded so any instructions following it will be properly situated in RAM.
        // uint8_t memory_map[4096]; 
        
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


        // In order to decode the opcode an array of function pointers were used.
        // To achieve minimum of space taken by this array the following 4 types were found:
        // -> The first digit only has one unique instruction:
                // $1nnn $2nnn $3xkk $4xkk $5xy0 $6xkk $7xkk $9xy0 $Annn $Bnnn $Cxkk $Dxyn
        // -> The first digit repeats but the last digit is unique:
                // $8xy0 $8xy1 $8xy2 $8xy3 $8xy4 $8xy5 $8xy6 $8xy7 $8xyE
        // -> The first three digits are $00E but the fourth digit is unique: 
                // $00E0 $00EE
        // -> The first digit repeats but the last two digits are unique: 
                // $ExA1 $Ex9E $Fx07 $Fx0A $Fx15 $Fx18 $Fx1E $Fx29 $Fx33 $Fx55 $Fx65


        // the tables have + 1 because the XX insctruction is stored in XX and not in XX-1 (Example 00EE is stored on index E so the size needs to be E+1) It is a little inneficiente (in memory storage) but a good approach instead of using the old switch statement

        // this table responsible for first digit
        void(Chip8::*table[0xD  + 1])(); // we need to say it takes a function pointer belonging to the class Chip8

        // in the not unique instruction case:
        // main table -> sub table -> sub_table_function() -> non_unique_instruction()
        void(Chip8::*table8[0xE + 1])();
        void Table8();

        void(Chip8::*table0[0xE + 1])();
        void Table0();

        void(Chip8::*tableE[0x9E + 1])();
        void TableE();

        void(Chip8::*tableF[0x65 + 1])();
        void TableF();



};




#endif