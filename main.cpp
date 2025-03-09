#include <iostream>
#include <chrono>
#include "chip8.h"
#include "screen.h"
#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    Chip8 chip8;
    // const char* ROM_FILENAME = "ibm_logo_testrom.ch8"; // Passed
    // const char* ROM_FILENAME = "chip8-logo_test_rom.ch8"; // Passed
    // const char* ROM_FILENAME = "corax_test_rom.ch8"; // Passed
    const char* ROM_FILENAME = "flags_test_rom.ch8"; // Doesnt work
    // const char* ROM_FILENAME = "spaceinvaders.ch8"; // Doesnt work


    chip8.initialize();
    chip8.loadRom(ROM_FILENAME);
    
    // for(int r=0; r < 500; r++){
    //     std::cout << chip8.display[0x0u+r];
    // }
    Screen screen(640*2,320*2);
    screen.initialize();
    // screen.draw(chip8.display);
    // screen.drawDebugger();

    bool quit = false;
    SDL_Event event;

    int cycles = 0;
    // while(cycles < 238){ chip8.Cycle(); cycles++;}
    while (!quit)
    {
        // chip8.Cycle();
        // screen.draw(chip8.display);
        // screen.drawDebugger(chip8);
        // screen.render();
        if(SDL_WaitEvent(&event)){
            if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN){
                chip8.Cycle();
                screen.draw(chip8.display);
                screen.drawDebugger(chip8);
                screen.render();
            }
            if(event.key.keysym.sym == SDLK_2){
                quit = true;
            }
        }
    }
    





    // auto lastCycleTime = std::chrono::high_resolution_clock::now();

    // int cycles = 0;
    // while(cycles < 200){
    //     auto currentTime = std::chrono::system_clock::now();
    //     float difference = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count(); //count() return em segundos
        
    //     SDL_Event event;

        // if(SDL_WaitEvent(&event)){
        //     if(event.key.keysym.sym == SDLK_q){
        //         chip8.Cycle();
        //         screen.draw(chip8.display);
        //         screen.drawDebugger(chip8);
        //         screen.render();
        //         cycles++;
        //     }else if(event.type == SDL_QUIT){
        //         return 0;
        //     }
        // }
        
        // if(difference > 1){
        //     lastCycleTime = currentTime;
        //     chip8.Cycle();
        //     screen.draw(chip8.display);
        //     screen.drawDebugger(chip8);
        //     screen.render();
        //     cycles++;
        //     // for(int coll=0; coll < 32; coll++){
        //     //     for(int rr=0; rr < 64; rr++){
        //     //         std::cout << unsigned(chip8.display[rr*64+coll]);
        //     //     }
        //     //     std::cout << "\n";
        //     // }
        //     // std::cout << "-----------------\n"; 
        // }
    // }

    // for(int y=0; y < 32; y++){
    //     for(int x=0; x < 64; x++){
    //         std::cout << unsigned(chip8.display[y*64+x]);
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "-----------------\n"; 
    // for(int r=0; r < 2048; r++){
    //     std::cout << chip8.display[r];
    // }
    // screen.draw(chip8.display);
    // SDL_Delay(2000);
    return 0;
}