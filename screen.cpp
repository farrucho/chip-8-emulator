#include "screen.h"
#include <iostream>

void Screen::initialize(){
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
	}
    
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

    if( window == NULL ){
        std::cout << "Window could not be created! SDL_Error: %s\n"<< SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

}

void Screen::draw(uint8_t display[]){
    
    int spriteW = SCREEN_WIDTH/64;
    int spriteH = SCREEN_HEIGHT/32;
    
    for(int r=0; r < 64; r++){
        for(int c=0; c < 32; c++){
            SDL_SetRenderDrawColor(renderer, display[r*c], display[r*c], display[r*c], 0);
            // std::cout << display[r*c] << std::endl;
            // if(display[r*c] == 0x1u){
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            // }else{
            //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            // }
            for(int i=0; i < spriteW; i++){
                for(int j=0; j < spriteH; j++){
                    SDL_RenderDrawPoint(renderer, r*spriteW + i, c*spriteH + j);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}
