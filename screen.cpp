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
    
    for(int x=0; x < 64; x++){
        for(int y=0; y < 32; y++){
            SDL_SetRenderDrawColor(renderer, display[y*64+x]*255, display[y*64+x]*255, display[y*64+x]*255, 0);
            // std::cout << display[r*c] << std::endl;
            // if(display[r*c] == 0x1u){
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            // }else{
            //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            // }
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            // SDL_RenderDrawPoint(renderer,x*spriteW, y*spriteH);
            for(int xoff=0; xoff < spriteW; xoff++){
                for(int yoff=0; yoff < spriteH; yoff++){
                    SDL_RenderDrawPoint(renderer,x*spriteW + xoff, y*spriteH + yoff);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}
