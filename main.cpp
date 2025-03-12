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
    // const char* ROM_FILENAME = "flags_test_rom.ch8"; // PASSED
    // const char* ROM_FILENAME = "keypad_test_rom.ch8"; // PASSED
    const char* ROM_FILENAME = "tetris.ch8"; // Works
    // const char* ROM_FILENAME = "astro_dodge.ch8"; // Works
    // const char* ROM_FILENAME = "pong_1player.ch8"; // Works


    
    chip8.initialize();
    chip8.loadRom(ROM_FILENAME);
    
    Screen screen(640*2,320*2);
    screen.initialize();

    
    
    bool quit = false;
    auto lastCycleTime = std::chrono::steady_clock::now();
    
    while (!quit)
    {
        auto currentTime = std::chrono::steady_clock::now();
        float difference = std::chrono::duration<float, std::milli>(currentTime - lastCycleTime).count(); //count() return in ms
        


        SDL_Event event;
        while( SDL_PollEvent( &event ) ){
            // Layout Used:
            // Keypad       Keyboard
            // +-+-+-+-+    +-+-+-+-+
            // |1|2|3|C|    |1|2|3|4|
            // +-+-+-+-+    +-+-+-+-+
            // |4|5|6|D|    |Q|W|E|R|
            // +-+-+-+-+ => +-+-+-+-+
            // |7|8|9|E|    |A|S|D|F|
            // +-+-+-+-+    +-+-+-+-+
            // |A|0|B|F|    |Z|X|C|V|
            // +-+-+-+-+    +-+-+-+-+
            if(event.type == SDL_KEYDOWN){
                // std::cout << "clicou tecla"  << std::endl;;
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_1:
                    chip8.keyboard[0x1] = 1;
                    break;
                case SDLK_2:
                    chip8.keyboard[0x2] = 1;
                    break;
                case SDLK_3:
                    chip8.keyboard[0x3] = 1;
                    break;
                case SDLK_4:
                    chip8.keyboard[0XC] = 1;
                    break;
                case SDLK_q:
                    chip8.keyboard[0x4] = 1;
                    break;
                case SDLK_w:
                    chip8.keyboard[0x5] = 1;
                    break;
                case SDLK_e:
                    chip8.keyboard[0x6] = 1;
                    break;
                case SDLK_r:
                    chip8.keyboard[0xD] = 1;
                    break;
                case SDLK_a:
                    chip8.keyboard[0x7] = 1;
                    break;
                case SDLK_s:
                    chip8.keyboard[0x8] = 1;
                    break;
                case SDLK_d:
                    chip8.keyboard[0x9] = 1;
                    break;
                case SDLK_f:
                    chip8.keyboard[0xE] = 1;
                    break;
                case SDLK_z:
                    chip8.keyboard[0xA] = 1;
                    break;
                case SDLK_x:
                    chip8.keyboard[0x0] = 1;
                    break;
                case SDLK_c:
                    chip8.keyboard[0xB] = 1;
                    break;
                case SDLK_v:
                    chip8.keyboard[0xF] = 1;
                    break;

                default:
                    break;
                }
            }
            else if(event.type == SDL_KEYUP){
                // std::cout << "largou tecla"  << std::endl;
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.keyboard[0x1] = 0;
                    break;
                case SDLK_2:
                    chip8.keyboard[0x2] = 0;
                    break;
                case SDLK_3:
                    chip8.keyboard[0x3] = 0;
                    break;
                case SDLK_4:
                    chip8.keyboard[0XC] = 0;
                    break;
                case SDLK_q:
                    chip8.keyboard[0x4] = 0;
                    break;
                case SDLK_w:
                    chip8.keyboard[0x5] = 0;
                    break;
                case SDLK_e:
                    chip8.keyboard[0x6] = 0;
                    break;
                case SDLK_r:
                    chip8.keyboard[0xD] = 0;
                    break;
                case SDLK_a:
                    chip8.keyboard[0x7] = 0;
                    break;
                case SDLK_s:
                    chip8.keyboard[0x8] = 0;
                    break;
                case SDLK_d:
                    chip8.keyboard[0x9] = 0;
                    break;
                case SDLK_f:
                    chip8.keyboard[0xE] = 0;
                    break;
                case SDLK_z:
                    chip8.keyboard[0xA] = 0;
                    break;
                case SDLK_x:
                    chip8.keyboard[0x0] = 0;
                    break;
                case SDLK_c:
                    chip8.keyboard[0xB] = 0;
                    break;
                case SDLK_v:
                    chip8.keyboard[0XF] = 0;
                    break;

                default:
                    break;
                }
            }
        }
        
        if(difference > 1){
            chip8.Cycle();
            screen.draw(chip8.display);
            screen.drawDebugger(chip8);
            screen.render();
            lastCycleTime = std::chrono::steady_clock::now();
        }
    }



    return 0;
}