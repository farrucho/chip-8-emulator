#include <cstdint>
#include <fstream>
#include "chip8.h"
#include <random>
#include <iostream>

const unsigned int FONTSET_SIZE = 16*5;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200;

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

    // This fontset should be loaded in memory location 0x50 and onwards (until 0x64 in this fontset case).
    for(int i = 0; i < FONTSET_SIZE; i++)
    {
        memory_map[FONTSET_START_ADDRESS+i] = fontset[i];
    }

    
    // define the tables and subtables for the opcode

    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::JP_addr;
    table[0x2] = &Chip8::CALL_addr;
    table[0x3] = &Chip8::SE_Vx_byte;
    table[0x4] = &Chip8::SNE_Vx_byte;
    table[0x5] = &Chip8::SE_Vx_Vy;
    table[0x6] = &Chip8::LD_Vx_byte;
    table[0x7] = &Chip8::ADD_Vx_byte;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::SNE_Vx_Vy;
    table[0xA] = &Chip8::LD_I_addr;
    table[0xB] = &Chip8::JP_V0_addr;
    table[0xC] = &Chip8::RND_Vx_byte;
    table[0xD] = &Chip8::DRW_Vx_Vy_nibble;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;


    table8[0x0] = &Chip8::LD_Vx_Vy;
    table8[0x1] = &Chip8::OR_Vx_Vy;
    table8[0x2] = &Chip8::AND_Vx_Vy;
    table8[0x3] = &Chip8::XOR_Vx_Vy;
    table8[0x4] = &Chip8::ADD_Vx_Vy;
    table8[0x5] = &Chip8::SUB_Vx_Vy;
    table8[0x6] = &Chip8::SHR_Vx_Vy;
    table8[0x7] = &Chip8::SUBN_Vx_Vy;
    table8[0xE] = &Chip8::SHL_Vx_Vy;

    table0[0x0] = &Chip8::CLS;
    table0[0xE] = &Chip8::RET;

    tableE[0x9E] = &Chip8::SKP_Vx;
    tableE[0xA1] = &Chip8::SKNP_Vx;
    
    tableF[0x07] = &Chip8::LD_Vx_DT;
    tableF[0x0A] = &Chip8::LD_Vx_K;
    tableF[0x15] = &Chip8::LD_DT_Vx;
    tableF[0x18] = &Chip8::LD_ST_Vx;
    tableF[0x1E] = &Chip8::ADD_I_Vx;
    tableF[0x29] = &Chip8::LD_F_Vx;
    tableF[0x33] = &Chip8::LD_B_Vx;
    tableF[0x55] = &Chip8::LD_I_Vx;
    tableF[0x65] = &Chip8::LD_Vx_I;

}


void Chip8::Cycle(){
    // get opcode
    // Remember: In memory, the first byte of each instruction should be located at an even addresses.
    opcode = memory_map[pc] << 8u | memory_map[pc+1]; // with this we get the 4 bytes opcode

    // increment instruction to execute
    pc += 2;


    // execute instruction here
    // std::cout << "Instruction: " << ((opcode & 0XF000u) >> 12u) << std::endl;
    ((*this).*(table[(opcode & 0XF000u) >> 12u]))();

    
    
    // The delay timer is active whenever the delay timer register (DT) is non-zero. 
    //This timer does nothing more than subtract 1 from the value of DT at a rate of 60Hz. When DT reaches 0, it deactivates.
    if(delay_timer > 0){
        delay_timer -= 1;
    }
    
    // The sound timer is active whenever the sound timer register (ST) is non-zero. This timer also decrements at a rate of 60Hz, however, as long as ST's value is greater than zero, the Chip-8 buzzer will sound. When ST reaches zero, the sound timer deactivates.
    if(sound_timer > 0){
        delay_timer -= 1;
    }
}

void Chip8::Table8(){
    // choice based on least significant opcode digit
    // return adequated pointer
    ((*this).*(table8[opcode & 0x000F]))();
}

void Chip8::Table0(){
    // choice based on least significant opcode digit
    // return adequated pointer
    ((*this).*(table0[opcode & 0x000F]))();
}

void Chip8::TableE(){
    // choice based on least significant opcode digit
    // return adequated pointer
    ((*this).*(tableE[opcode & 0x00FF]))();
}

void Chip8::TableF(){
    // choice based on least significant opcode digit
    // return adequated pointer
    ((*this).*(tableF[opcode & 0x00FF]))();
}

void Chip8::loadRom(char const* filename){    
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
    // memset(display, 0, sizeof(display));
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
    std::cout << unsigned(V[x]) << "  " << unsigned(kk) << std::endl;
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
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    if(V[x] == V[y]){
        pc += 2; // skip next instruction and put counter to next instruction address 
    }
    
}

void Chip8::LD_Vx_byte(){
    // 6xkk
    // Set Vx = kk.
    // The interpreter puts the value kk into register Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    V[x] = kk;

}
void Chip8::ADD_Vx_byte(){
    // 7xkk
    // Set Vx = Vx + kk.
    // Adds the value kk to the value of register Vx, then stores the result in Vx. 

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    V[x] = kk + V[x];
}

void Chip8::LD_Vx_Vy(){
    // 8xy0
    // Set Vx = Vy.
    // Stores the value of register Vy in register Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    V[x] = V[y];
}

void Chip8::OR_Vx_Vy(){
    // 8xy1
    // Set Vx = Vx OR Vy.

    // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corresponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    V[x] = V[x] | V[y];
}

void Chip8::AND_Vx_Vy(){
    // 8xy2
    // Set Vx = Vx AND Vy.

    // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. 

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    V[x] = V[x] & V[y];
}

void Chip8::XOR_Vx_Vy(){
    // 8xy3
    // Set Vx = Vx XOR Vy.

    // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    V[x] = V[x] ^ V[y];
    
}

void Chip8::ADD_Vx_Vy(){
    // 8xy4
    // Set Vx = Vx + Vy, set VF = carry.

    // The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    
    uint16_t sum = V[x] + V[y];
    
    V[x] = sum & 0xFFu; // so no overflow error

    V[0xFu] = sum >> 8u; // overflow bit
    
}

void Chip8::SUB_Vx_Vy(){
    // 8xy5
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    
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
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    
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
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    
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
    uint8_t y = (opcode & 0x00F0u) >> 4u;

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


void Chip8::JP_V0_addr(){
    // Bnnn
    // Jump to location nnn + V0.
    // The program counter is set to nnn plus the value of V0.

    uint16_t address = opcode & 0x0FFFu;
    pc = address + V[0x0u];
}

void Chip8::RND_Vx_byte(){
    // Cxkk
    // Set Vx = random byte AND kk.
    // The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,255u);

    V[x] = distribution(generator) & byte;
}

void Chip8::DRW_Vx_Vy_nibble(){
    // Dxyn
    // Display n-byte sprite (n rows) starting at memory location I at (Vx, Vy), set VF = collision. Each row of a sprite is made of 8 bits aka 1 byte.
    // The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    uint8_t n = (opcode & 0x000Fu); // rows

    // uint8_t xPos = V[x] % 64;
    // uint8_t yPos = V[y] % 32;
    // std::cout << "(" << unsigned(V[x]) << ", " << unsigned(V[y]) << ")\n";

    V[0xFu] = 0;

    // for(int ki = 0; ki < 32; ki++){
    //     for(int kj = 0; kj < 64; kj++){
    //         std::cout << unsigned(ki*64 + kj) << " ";
    //     }
    //     std::cout << "\n";
    // }

    for(int spriteRow = 0; spriteRow < n; spriteRow++){
        uint8_t spritebyte = memory_map[I + spriteRow];
        // std::cout << "spritebyte: " << unsigned(spritebyte) << std::endl;

        for(int spriteCol = 0; spriteCol < 8; spriteCol++){ // every byte row has 8 collumn bits
            // get current bit to write to display (starting from left of course)
            uint8_t spritepixel = (spritebyte & (0x80u >> spriteCol)) >> (7-spriteCol);
            // uint16_t pixelPostion = (V[x]+i)*64 + (V[y] + col);
            uint16_t pixelPostion = ((V[y]%32) +spriteRow)*64 + ((V[x]%64) + spriteCol);
            
            uint8_t* screenPixel = &display[pixelPostion];
            
            // std::cout << "----------------------------" << std::endl;
            // for(int ki = 0; ki < 32; ki++){
                //     for(int kj = 0; kj < 64; kj++){
                    //         std::cout << unsigned(ki*64 + kj) << " ";
                    //     }
                    //     std::cout << "\n";
                    // }
                    // std::cout << "----------------------------" << std::endl;
                    
            // *screenPixel ^= spritepixel;
                    
            // std::cout << unsigned(spritepixel) << "  " << unsigned(pixelPostion) << "  " << unsigned(*screenPixel) << std::endl;
            
            // if(unsigned(pixelPostion) > 2048){
            //     throw std::invalid_argument( "pixel positon too big" );
            // }


            // SpritePixel ScreenPixel XOR
            // 0 0 0
            // 0 1 1
            // 1 0 1  <-
            // 1 1 0  <-
            // -> so atualiza caso spritepixel == 1
            if(spritepixel != 0){
                if(*screenPixel == 1){
                    V[0xFu] = 1;
                }
                
                // *screenPixel = ~(spritepixel);
            }
            *screenPixel ^= spritepixel;
        }
        // std::cout << std::endl;
    } 
}

void Chip8::SKP_Vx(){
    // Ex9E
    // Skip next instruction if key with the value of Vx is pressed.
    // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;

    uint8_t key = V[x];

    if(keyboard[key]){
        pc += 2;
    }

    
}

void Chip8::SKNP_Vx(){
    // ExA1
    // Skip next instruction if key with the value of Vx is not pressed.
    // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;

    uint8_t key = V[x];

    if(!keyboard[key]){
        pc += 2;
    }
}

void Chip8::LD_Vx_DT(){
    // Fx07
    // Set Vx = delay timer value.
    // The value of DT is placed into Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    V[x] = delay_timer;
}

void Chip8::LD_Vx_K(){
    // Fx0A
    // Wait for a key press, store the value of the key in Vx.
    // All execution stops until a key is pressed, then the value of that key is stored in Vx.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < 15; i++){
        if(keyboard[i]){
            V[x] = i;
            return;
        }
    }
    pc -= 2;
}

void Chip8::LD_DT_Vx(){
    // Fx15
    // Set delay timer = Vx.
    // DT is set equal to the value of Vx.
    
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[x];
}

void Chip8::LD_ST_Vx(){
    // Fx18
    // Set sound timer = Vx.
    // ST is set equal to the value of Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    sound_timer = V[x];
}

void Chip8::ADD_I_Vx(){
    // Fx1E
    // Set I = I + Vx.
    // The values of I and Vx are added, and the results are stored in I.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    I = I + V[x];
}

void Chip8::LD_F_Vx(){
    // Fx29
    // Set I = location of sprite for digit Vx.
    // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    
    // See how we initialize fontset to understand
    // every sprite has 5 bytes
    I = FONTSET_START_ADDRESS + 5*V[x];
}

void Chip8::LD_B_Vx(){
    // Fx33
    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
    // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.

    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t value = V[x];

    // Example: 345
    memory_map[I] = value % 10; // 5
    value = value / 10; // 34.5 = 34 (truncated )
    memory_map[I] = value % 10; // 4
    value = value / 10;// 3.4 = 3
    memory_map[I] = value % 10; // 3
}

void Chip8::LD_I_Vx(){
    // Fx55
    // Store registers V0 through Vx in memory starting at location I.
    // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.

    uint8_t x = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < x; i++){
        memory_map[I + i] = V[i];
    }
}

void Chip8::LD_Vx_I(){
    // Fx65
    // Read registers V0 through Vx from memory starting at location I.
    // The interpreter reads values from memory starting at location I into registers V0 through Vx.

    uint8_t x = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < x; i++){
        V[i] = memory_map[I + i];
    }
}
