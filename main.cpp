#include "chip8.h"
#include <iostream>


int main()
{
    Chip8 chip8;
    const char* ROM_FILENAME = "spaceinvaders.ch8";


    chip8.initialize();
    chip8.loadRom(ROM_FILENAME);

    




    return 0;
}