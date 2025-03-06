#include "screen.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <string>

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
    char opcodeText[4]; // 4 opcode chars
    sprintf(opcodeText, "%04X", opcode); // Hexadecimal ABCD

    // draw blue screen
    SDL_SetRenderDrawColor((*this).renderer, 1, 0, 185, 0);
    SDL_Rect fillRectRightTop = {xDisplay, 0, SCREEN_WIDTH - xDisplay, yDisplay}; 
    SDL_RenderFillRect((*this).renderer, &fillRectRightTop);

    
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


    // draw V registers
    SDL_SetRenderDrawColor((*this).renderer, 1, 0, 165, 0);
    SDL_Rect fillRectRightBottom = {xDisplay, yDisplay, SCREEN_WIDTH - xDisplay, SCREEN_HEIGHT - yDisplay}; 
    SDL_RenderFillRect((*this).renderer, &fillRectRightBottom);


    std::string Vtext = "";
    
    char Vvalue[8];
    for(int i=0; i < 16; i++){
        Vtext = "V";
        Vtext += std::to_string(i); 
        if(i < 10){
            Vtext += " ";
        }
        Vtext += ": ";
        sprintf(Vvalue, "%02X", chip8.getV(i)); // Hexadecimal ABCD
        Vtext += Vvalue;
        // Vtext += "\n";
        SDL_Surface* surfaceMessageVregs = TTF_RenderText_Solid(font, Vtext.c_str(), White);
        SDL_Texture* MessageVregs = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageVregs);
        
        SDL_Rect Message_rectVregs;
        Message_rectVregs.x = xDisplay;
        Message_rectVregs.y = yDisplay + i*(SCREEN_HEIGHT-yDisplay)/16;
        Message_rectVregs.w = surfaceMessageVregs->w;
        Message_rectVregs.h = surfaceMessageVregs->h;
        
        SDL_RenderCopy((*this).renderer, MessageVregs, NULL, &Message_rectVregs);
        SDL_FreeSurface(surfaceMessageVregs);
        SDL_DestroyTexture(MessageVregs);
    }
    // Vtext.pop_back();
    // std::cout << "--" << Vtext << "--" << std::endl;




    // SDL_FreeSurface(surfaceMessageVregs);
    // SDL_DestroyTexture(MessageVregs);
    // std::cout << Vtext << std::endl;

    // chip8.getOpcodeFunctionPtr();
    // std::cout << ((chip8).*(table[(opcode & 0XF000u) >> 12u]))() << std::endl;

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
