#include <iostream>
#include <chrono>
#include "chip8.h"
#include "screen.h"
#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    Chip8 chip8;
    // const char* ROM_FILENAME = "ibm_logo_testrom.ch8"; // PASSED
    // const char* ROM_FILENAME = "chip8-logo_test_rom.ch8"; // PASSED
    // const char* ROM_FILENAME = "corax_test_rom.ch8"; // PASSED
    const char* ROM_FILENAME = "flags_test_rom.ch8"; // PASSED
    // const char* ROM_FILENAME = "spaceinvaders.ch8"; // Doesnt work
    // const char* ROM_FILENAME = "tetris.ch8"; // Doesnt work
    // const char* ROM_FILENAME = "pong.ch8"; // Doesnt work

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
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    int cycles = 0;
    
    // while(cycles < 1400){ chip8.Cycle(); cycles++;}
    while (!quit)
    {
        auto currentTime = std::chrono::system_clock::now();
        float difference = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count(); //count() return em segundos
        
        SDL_Event event;
        if(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                std::cout << "clicou tecla";
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_1:
                    chip8.keyboard[0] = 1;
                    break;
                case SDLK_2:
                    chip8.keyboard[1] = 1;
                    break;
                case SDLK_3:
                    chip8.keyboard[2] = 1;
                    break;
                case SDLK_4:
                    chip8.keyboard[3] = 1;
                    break;
                case SDLK_q:
                    chip8.keyboard[4] = 1;
                    break;
                case SDLK_w:
                    chip8.keyboard[5] = 1;
                    break;
                case SDLK_e:
                    chip8.keyboard[6] = 1;
                    break;
                case SDLK_r:
                    chip8.keyboard[7] = 1;
                    break;
                case SDLK_a:
                    chip8.keyboard[8] = 1;
                    break;
                case SDLK_s:
                    chip8.keyboard[9] = 1;
                    break;
                case SDLK_d:
                    chip8.keyboard[10] = 1;
                    break;
                case SDLK_f:
                    chip8.keyboard[11] = 1;
                    break;
                case SDLK_z:
                    chip8.keyboard[12] = 1;
                    break;
                case SDLK_x:
                    chip8.keyboard[13] = 1;
                    break;
                case SDLK_c:
                    chip8.keyboard[14] = 1;
                    break;
                case SDLK_v:
                    chip8.keyboard[15] = 1;
                    break;
                
                default:
                    break;
                }
            }else if(event.type == SDL_KEYUP){
                std::cout << "largou tecla";
                switch (event.key.keysym.sym)
                {
                    case SDLK_1:
                    chip8.keyboard[0] = 0;
                    break;
                case SDLK_2:
                    chip8.keyboard[1] = 0;
                    break;
                case SDLK_3:
                    chip8.keyboard[2] = 0;
                    break;
                case SDLK_4:
                    chip8.keyboard[3] = 0;
                    break;
                case SDLK_q:
                    chip8.keyboard[4] = 0;
                    break;
                case SDLK_w:
                    chip8.keyboard[5] = 0;
                    break;
                case SDLK_e:
                    chip8.keyboard[6] = 0;
                    break;
                case SDLK_r:
                    chip8.keyboard[7] = 0;
                    break;
                case SDLK_a:
                    chip8.keyboard[8] = 0;
                    break;
                case SDLK_s:
                    chip8.keyboard[9] = 0;
                    break;
                case SDLK_d:
                    chip8.keyboard[10] = 0;
                    break;
                case SDLK_f:
                    chip8.keyboard[11] = 0;
                    break;
                case SDLK_z:
                    chip8.keyboard[12] = 0;
                    break;
                case SDLK_x:
                    chip8.keyboard[13] = 0;
                    break;
                case SDLK_c:
                    chip8.keyboard[14] = 0;
                    break;
                case SDLK_v:
                    chip8.keyboard[15] = 0;
                    break;
                }
            }
            // if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN){
            //     chip8.Cycle();
            //     screen.draw(chip8.display);
            //     screen.drawDebugger(chip8);
            //     screen.render();
            // }
            // if(event.key.keysym.sym == SDLK_2){
            //     quit = true;
            // }
        }

        if(difference > 1){
            chip8.Cycle();
            screen.draw(chip8.display);
            screen.drawDebugger(chip8);
            screen.render();
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