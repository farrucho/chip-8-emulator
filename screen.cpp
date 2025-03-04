#include "screen.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cstdint>

void Screen::initialize(){
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
	}
    
    (*this).window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

    if( (*this).window == NULL ){
        std::cout << "Window could not be created! SDL_Error: %s\n"<< SDL_GetError() << std::endl;
    }

    (*this).renderer = SDL_CreateRenderer((*this).window, -1, SDL_RENDERER_ACCELERATED);
    if (!(*this).renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow((*this).window);
        SDL_Quit();
    }

}

void Screen::render(){
    SDL_RenderPresent((*this).renderer);
}

void Screen::drawDebugger(Chip8 chip8){
    int spriteW = 0.5*SCREEN_WIDTH/64;
    int spriteH = 0.5*SCREEN_HEIGHT/32;
    int xDisplay = 0.5*SCREEN_WIDTH;
    int yDisplay = 0.5*SCREEN_HEIGHT;

    if ( TTF_Init() < 0 ) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    }
    TTF_Font* font = TTF_OpenFont("./PressStart2P-Regular.ttf", spriteW*2);
    SDL_Color White = {123, 125, 201};

    uint16_t opcode = chip8.getOpcode();
    char opcodeText[5]; // 4 opcode chars + + '\0'

    // Converter para string hexadecimal
    sprintf(opcodeText, "%04X", opcode); // Formato "1A2B", por exemplo

    SDL_SetRenderDrawColor((*this).renderer, 1, 0, 185, 0);
    for(int x=xDisplay; x < SCREEN_WIDTH; x++){
        for(int y=0; y < yDisplay; y++){
            SDL_RenderDrawPoint((*this).renderer,x, y);
        }
    }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, opcodeText, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = xDisplay;
    Message_rect.y = 0;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    SDL_RenderCopy((*this).renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

// // 4. Atualizar a tela **uma única vez no final**
// SDL_RenderPresent((*this).renderer);
}


void Screen::draw(uint8_t display[]){
    int spriteW = 0.5*SCREEN_WIDTH/64;
    int spriteH = 0.5*SCREEN_HEIGHT/32;
    
    for(int x=0; x < 64; x++){
        for(int y=0; y < 32; y++){
            SDL_SetRenderDrawColor((*this).renderer, display[y*64+x]*255, display[y*64+x]*255, display[y*64+x]*255, 0);
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
                    SDL_RenderDrawPoint((*this).renderer,x*spriteW + xoff, y*spriteH + yoff);
                }
            }
        }
    }

    // SDL_RenderPresent(renderer);
}
