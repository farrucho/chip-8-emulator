#include <cstdint>
#include <fstream>
#include "chip8.h"


void Chip8::initialize(){

    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    // IMPLEMENTAR:
    // clear display, stack, V registers, memory and timers

    // Load fontset sprites to memory
    /*
    Sprite "0"	Binary	    Hex
        ****    11110000    0xF0
        *  *    10010000    0x90
        *  *    10010000    0x90
        *  *    10010000    0x90
        ****    11110000    0xF0
    */

    const unsigned int FONTSET_SIZE = 16*5;
    const unsigned int FONTSET_START_ADDRESS = 0x50;
    uint8_t fontset[FONTSET_SIZE] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // This fontset should be loaded in memory location 0x50 and onwards (until 0x64 in this fontset case).
    for(int i = 0; i < FONTSET_SIZE; i++)
    {
        memory_map[FONTSET_START_ADDRESS+i] = fontset[i];
    }

    

}

void Chip8::loadRom(char const* filename){
    // load ROM begining at 0x200
    const unsigned int START_ADDRESS = 0x200;

    
    std::ifstream romFile(filename, std::ios::binary);
    // when opening a file it creates a file pointer on the begining to dictate where to read or write

    if(romFile.is_open()){
        romFile.seekg(0, std::ios::end); // move file pointer to the end
        std::streampos size = romFile.tellg(); // get position of file pointer in bytes
        romFile.seekg(0, std::ios::beg); // move file pointer to begining so the work can start in the natural start of the file
    
        // size is not a int/long type so it must be forced convertible with new char[size] int this way the array is created
        char* buffer = new char[size];

        romFile.read(buffer, size); // reads the number size bytes and stores all of them in the buffer
        // now all bytes are in the buffer and we can acess it

        // we are redding from 0 to 4096 to bytes so we need to have at least a variable with 
        // 4096 values or 12 bits or 2 bytes, long or int will serve
        for(int i = 0; i < size;i++){ 
            memory_map[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
        // delete[] instead of only delete so it deletes properly the array
        romFile.close();
    }

}


void Chip8::SYS_addr(){
    // 0nnn
    // Jump to a machine code routine at nnn.
    // This routine would be written in the machine language of the computer’s CPU; on the original COSMAC VIP and the ETI-660, this was 1802 machine code, and on the DREAM 6800, M6800 code. Unless you’re making an emulator for either of those computers, skip this one.
}

void Chip8::CLS(){
    // 00E0
    // Clear the display.
    std::fill(std::begin(display), std::end(display), 0);
}

void Chip8::RET(){
    // opcode == 00EE
    // Return from a subroutine.
    // The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.

    // sp is pointing to next available empty position so we need to get the last called address of the stack (aka the subroutine)
    sp--;
    pc = stack[sp];
}

void Chip8::JP_addr(){
    // 1nnn
    // Jump to location nnn.
    // The interpreter sets the program counter to nnn.
    // grab the address nnn (12 most right) bits with a bitwise and operation
    // The left 4 bits are defined as 0 since they are not relevant
    uint16_t address = opcode & 0x0FFFu;

    pc = address;
}

void Chip8::CALL_addr(){
    // 2nnn
    // Call subroutine at nnn.

    // set subroutine on stack
    stack[sp] = pc;
    sp++; // point to next available empty position on stack

    // call subroutine
    // grab the address nnn (12 most right) bits with a bitwise and operation
    // The left 4 bits are defined as 0 since they are not relevant
    uint16_t address = opcode & 0x0FFFu; // the ending 'u' ensures 0x0FFF is unsigned int (safer)
    pc = address;
}

void Chip8::SE_Vx_byte(){
    // 3xkk
    // Skip next instruction if Vx = kk.
    // The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;// pick x (we chose uint8 because we dont have uint4_t)
    uint8_t kk = opcode & 0x00FFu;

    if(V[x] == kk){
        pc += 2; // skip next instruction and put counter to next instruction address 
    }

}


void Chip8::SNE_Vx_byte(){
    // 4xkk
    // Skip next instruction if Vx != kk.
    // The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if(V[x] != byte){
        pc += 2; // skip next instruction and put counter to next instruction address 
    }
}

void Chip8::SE_Vx_Vy(){
    // 5xy0
    // Skip next instruction if Vx = Vy.
    // The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;

    if(V[x] == V[y]){
        pc += 2; // skip next instruction and put counter to next instruction address 
    }
    
}

void Chip8::LD_Vx_byte(){
    // 6xkk
    // Set Vx = kk.
    // The interpreter puts the value kk into register Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[x] = byte;

}
void Chip8::ADD_Vx_byte(){
    // 7xkk
    // Set Vx = Vx + kk.
    // Adds the value kk to the value of register Vx, then stores the result in Vx. 

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[x] = byte + V[x];
}

void Chip8::LD_Vx_Vy(){
    // 8xy0
    // Set Vx = Vy.
    // Stores the value of register Vy in register Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    V[x] = V[y];
}

void Chip8::OR_Vx_Vy(){
    // 8xy1
    // Set Vx = Vx OR Vy.

    // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corresponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    V[x] = V[x] | V[y];
}

void Chip8::AND_Vx_Vy(){
    // 8xy2
    // Set Vx = Vx AND Vy.

    // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. 

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    V[x] = V[x] & V[y];
}

void Chip8::XOR_Vx_Vy(){
    // 8xy3
    // Set Vx = Vx XOR Vy.

    // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;

    V[x] = V[x] ^ V[y];
    
}

void Chip8::ADD_Vx_Vy(){
    // 8xy4
    // Set Vx = Vx + Vy, set VF = carry.

    // The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    
    uint16_t sum = V[x] + V[y];
    
    V[x] = sum & 0xFFu; // so no overflow error

    V[0xFu] = sum >> 8u; // overflow bit
    
}

void Chip8::SUB_Vx_Vy(){
    // 8xy5
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    
    if (V[x] > V[y]){
        V[0xFu] = 1;
    }
    else{
        V[0xFu] = 0;
    }

    V[x] = V[x] - V[y];    
}

void Chip8::SHR_Vx_Vy(){
    // 8xy6
    // Set Vx = Vx SHR 1.
    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    
    
    V[0xFu] = (V[x] & 0x1u);
    
    V[x] = V[x] >> 1;
}

void Chip8::SUBN_Vx_Vy(){
    // 8xy7
    // Set Vx = Vy - Vx, set VF = NOT borrow.
    // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    
    if (V[x] > V[y]){
        V[0xFu] = 1;
    }
    else{
        V[0xFu] = 0;
    }

    V[x] = V[y] - V[x];
}

void Chip8::SHL_Vx_Vy(){
    // 8xyE
    // Set Vx = Vx SHL 1.
    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;
    
    V[0xFu] = (V[x] & 0x1u);

    if (V[x] & 0x1u){
        V[0xFu] = 1;
    }
    else{
        V[0xFu] = 0;
    }

    V[x] = V[x] << 1;
}

void Chip8::SNE_Vx_Vy(){
    // 9xy0
    // Skip next instruction if Vx != Vy.

    // The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x0F00u) >> 4u;

    if(V[x] != V[y]){
        pc += 2; // skip next instruction and put counter to next instruction address
    }
    
}

void Chip8::LD_I_addr(){
    // Annn 
    // Set I = nnn.

    // The value of register I is set to nnn.
    uint16_t address = opcode & 0x0FFFu;

    I = address;
}