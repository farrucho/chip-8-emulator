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

    
    // draw blue screen
    SDL_SetRenderDrawColor((*this).renderer, 1, 0, 185, 0);
    SDL_Rect fillRectRightTop = {xDisplay, 0, SCREEN_WIDTH - xDisplay, yDisplay}; 
    SDL_RenderFillRect((*this).renderer, &fillRectRightTop);
    
    // draw opcodes
    for(int i = 0; i < 10; i++){
        uint8_t* memory = chip8.memory_map;
        uint16_t pc = chip8.getPc();
        uint16_t currentOpcode = memory[pc + i*2] << 8u | memory[pc+i*2+1]; // displays next instructions
        char opcodeBuffer[4] = {};
        sprintf(opcodeBuffer, "%04X", currentOpcode); // Hexadecimal ABCD
        // std::cout << opcodeBuffer << std::endl;
        // char opcodeBuffer1[2] = {};
        // char opcodeBuffer2[2] = {};
        
        // sprintf(opcodeBuffer1, "%2X", memory[pc+i*2] & 0xF0); // Hexadecimal ABCD
        // sprintf(opcodeBuffer2, "%2X", memory[pc+i*2+1]& 0x0F); // Hexadecimal ABCD
        // std::string opcodeText = "";
        // opcodeText += opcodeBuffer1;
        // opcodeText += opcodeBuffer2;
        // std::cout << opcodeBuffer1 << std::endl;
        // std::cout << opcodeBuffer2 << std::endl;

        SDL_Surface* surfaceMessageOpcode = TTF_RenderText_Solid(font, opcodeBuffer, White);
        SDL_Texture* MessageOpcode = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageOpcode);

        SDL_Rect MessageOpcode_rect;
        MessageOpcode_rect.x = xDisplay;
        MessageOpcode_rect.y = i*yDisplay/10;
        MessageOpcode_rect.w = surfaceMessageOpcode->w;
        MessageOpcode_rect.h = surfaceMessageOpcode->h;
    
        SDL_RenderCopy((*this).renderer, MessageOpcode, NULL, &MessageOpcode_rect);
    
        SDL_FreeSurface(surfaceMessageOpcode);
        SDL_DestroyTexture(MessageOpcode);
    }

    // uint16_t opcode = chip8.getOpcode();
    // char opcodeText[4]; // 4 opcode chars
    // sprintf(opcodeText, "%04X", opcode); // Hexadecimal ABCD
    

    // SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, opcodeText, White);
    // SDL_Texture* Message = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessage);

    // SDL_Rect Message_rect;
    // Message_rect.x = xDisplay;
    // Message_rect.y = 0;
    // Message_rect.w = surfaceMessage->w;
    // Message_rect.h = surfaceMessage->h;

    // SDL_RenderCopy((*this).renderer, Message, NULL, &Message_rect);

    // SDL_FreeSurface(surfaceMessage);
    // SDL_DestroyTexture(Message);

    


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
        Vtext += ":";
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

    // draw I
    std::string Itext = "";
    Itext += "I: ";
    char Ivalue[8];
    sprintf(Ivalue, "%04X", chip8.getI()); // Hexadecimal ABCD
    Itext += Ivalue;
    SDL_Surface* surfaceMessageI = TTF_RenderText_Solid(font, Itext.c_str(), White);
    SDL_Texture* MessageI = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageI);

        
    SDL_Rect Message_rectI;
    Message_rectI.x = xDisplay + 1*(SCREEN_WIDTH-xDisplay)/4;;
    Message_rectI.y = yDisplay + 0*(SCREEN_HEIGHT-yDisplay)/16;
    Message_rectI.w = surfaceMessageI->w;
    Message_rectI.h = surfaceMessageI->h;
        
    SDL_RenderCopy((*this).renderer, MessageI, NULL, &Message_rectI);
    SDL_FreeSurface(surfaceMessageI);
    SDL_DestroyTexture(MessageI);

    // draw PC
    std::string PCtext = "";
    PCtext += "PC:";
    char PCvalue[8];
    sprintf(PCvalue, "%04X", chip8.getPc()); // Hexadecimal ABCD
    PCtext += PCvalue;
    SDL_Surface* surfaceMessagePC = TTF_RenderText_Solid(font, PCtext.c_str(), White);
    SDL_Texture* MessagePC = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessagePC);

        
    SDL_Rect Message_rectPC;
    Message_rectPC.x = xDisplay + 1*(SCREEN_WIDTH-xDisplay)/4;
    Message_rectPC.y = yDisplay + 2*(SCREEN_HEIGHT-yDisplay)/16;
    Message_rectPC.w = surfaceMessagePC->w;
    Message_rectPC.h = surfaceMessagePC->h;
        
    SDL_RenderCopy((*this).renderer, MessagePC, NULL, &Message_rectPC);
    SDL_FreeSurface(surfaceMessagePC);
    SDL_DestroyTexture(MessagePC);

    //draw sp
    std::string SPtext = "";
    SPtext += "SP:";
    char SPvalue[8];
    sprintf(SPvalue, "%02X", chip8.getSp()); // Hexadecimal ABCD
    SPtext += SPvalue;
    SDL_Surface* surfaceMessageSP = TTF_RenderText_Solid(font, SPtext.c_str(), White);
    SDL_Texture* MessageSP = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageSP);

        
    SDL_Rect Message_rectSP;
    Message_rectSP.x = xDisplay + 1*(SCREEN_WIDTH-xDisplay)/4;
    Message_rectSP.y = yDisplay + 4*(SCREEN_HEIGHT-yDisplay)/16;
    Message_rectSP.w = surfaceMessageSP->w;
    Message_rectSP.h = surfaceMessageSP->h;
        
    SDL_RenderCopy((*this).renderer, MessageSP, NULL, &Message_rectSP);
    SDL_FreeSurface(surfaceMessageSP);
    SDL_DestroyTexture(MessageSP);

    
    // draw stack
    std::string Stacktext = "";
    
    char Stackvalue[16];
    for(int i=0; i < 16; i++){
        Stacktext = "S";
        Stacktext += std::to_string(i); 
        if(i < 10){
            Stacktext += " ";
        }
        Stacktext += ":";
        sprintf(Stackvalue, "%04X", chip8.getStack(i)); // Hexadecimal ABCD
        Stacktext += Stackvalue;
        // Vtext += "\n";
        SDL_Surface* surfaceMessageStack = TTF_RenderText_Solid(font, Stacktext.c_str(), White);
        SDL_Texture* MessageStack = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageStack);
        
        SDL_Rect Message_rectStack;
        Message_rectStack.x = xDisplay + 2*(SCREEN_WIDTH-xDisplay)/4;
        Message_rectStack.y = yDisplay + i*(SCREEN_HEIGHT-yDisplay)/16;
        Message_rectStack.w = surfaceMessageStack->w;
        Message_rectStack.h = surfaceMessageStack->h;
        
        SDL_RenderCopy((*this).renderer, MessageStack, NULL, &Message_rectStack);
        SDL_FreeSurface(surfaceMessageStack);
        SDL_DestroyTexture(MessageStack);
    }

    // SDL_SetRenderDrawColor((*this).renderer, 1, 0, 165, 0);
    // SDL_Rect fillRectRightBottom = {xDisplay, yDisplay, SCREEN_WIDTH - xDisplay, SCREEN_HEIGHT - yDisplay}; 
    // SDL_RenderFillRect((*this).renderer, &fillRectRightBottom);


    // std::string Vtext = "";
    
    // char Vvalue[8];
    // for(int i=0; i < 16; i++){
    //     Vtext = "V";
    //     Vtext += std::to_string(i); 
    //     if(i < 10){
    //         Vtext += " ";
    //     }
    //     Vtext += ": ";
    //     sprintf(Vvalue, "%02X", chip8.getV(i)); // Hexadecimal ABCD
    //     Vtext += Vvalue;
    //     // Vtext += "\n";
    //     SDL_Surface* surfaceMessageVregs = TTF_RenderText_Solid(font, Vtext.c_str(), White);
    //     SDL_Texture* MessageVregs = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageVregs);
        
    //     SDL_Rect Message_rectVregs;
    //     Message_rectVregs.x = xDisplay;
    //     Message_rectVregs.y = yDisplay + i*(SCREEN_HEIGHT-yDisplay)/16;
    //     Message_rectVregs.w = surfaceMessageVregs->w;
    //     Message_rectVregs.h = surfaceMessageVregs->h;
        
    //     SDL_RenderCopy((*this).renderer, MessageVregs, NULL, &Message_rectVregs);
    //     SDL_FreeSurface(surfaceMessageVregs);
    //     SDL_DestroyTexture(MessageVregs);
    // }


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
