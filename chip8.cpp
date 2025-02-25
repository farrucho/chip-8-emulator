#include <cstdint>
#include <fstream>
#include "chip8.h"
#include <iostream>

void Chip8::initialize(){


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
        std::cout << size;

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

