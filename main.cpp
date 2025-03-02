#include <iostream>
#include <chrono>
#include "chip8.h"
#include "screen.h"
#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    Chip8 chip8;
    const char* ROM_FILENAME = "ibm_logo_testrom.ch8";


    chip8.initialize();
    chip8.loadRom(ROM_FILENAME);
    
    // for(int r=0; r < 500; r++){
    //     std::cout << chip8.display[0x0u+r];
    // }
    Screen screen(640,320);
    screen.initialize();
    screen.draw(chip8.display);


    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    int cycles = 0;
    while(cycles < 30){
        auto currentTime = std::chrono::system_clock::now();
        float difference = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count(); //count() return em segundos


        if(difference > 10){
            lastCycleTime = currentTime;
            chip8.Cycle();
            screen.draw(chip8.display);
            // for(int r=0; r < 5000; r++){
            //     std::cout << chip8.display[r];
            // }
            std::cout << "_" << std::endl;
            cycles++;
        }
    }

    // for(int r=0; r < 2048; r++){
    //     std::cout << chip8.display[r];
    // }
    // screen.draw(chip8.display);

    return 0;
}