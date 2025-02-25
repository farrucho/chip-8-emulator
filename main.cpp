#include "chip8.h"
#include <iostream>


int main()
{
    Chip8 chip8;
    const char* ROM_FILENAME = "spaceinvaders.ch8";

    chip8.loadRom(ROM_FILENAME);

    for(int i = 0; i < 10; i++){
        std::cout << chip8.memory_map[0x200+i] << std::endl;
    }





    return 0;
}