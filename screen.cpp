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


    // draw delay timer
    std::string DelayTimertext = "";
    DelayTimertext += "ST:";
    char DelayTimervalue[8];
    sprintf(DelayTimervalue, "%02X", chip8.getDelay()); // Hexadecimal ABCD
    DelayTimertext += DelayTimervalue;
    SDL_Surface* surfaceMessageDelayTimer = TTF_RenderText_Solid(font, DelayTimertext.c_str(), White);
    SDL_Texture* MessageDelayTimer = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageDelayTimer);

        
    SDL_Rect Message_rectDelayTimer;
    Message_rectDelayTimer.x = xDisplay + 1*(SCREEN_WIDTH-xDisplay)/4;
    Message_rectDelayTimer.y = yDisplay + 6*(SCREEN_HEIGHT-yDisplay)/16;
    Message_rectDelayTimer.w = surfaceMessageDelayTimer->w;
    Message_rectDelayTimer.h = surfaceMessageDelayTimer->h;
        
    SDL_RenderCopy((*this).renderer, MessageDelayTimer, NULL, &Message_rectDelayTimer);
    SDL_FreeSurface(surfaceMessageDelayTimer);
    SDL_DestroyTexture(MessageDelayTimer);

    
    
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

    std::string Keypadtext = "";
    
    char Keypadvalue[16];
    for(int i=0; i < 16; i++){
        Keypadtext = "K";
        Keypadtext += std::to_string(i); 
        if(i < 10){
            Keypadtext += " ";
        }
        Keypadtext += ":";
        sprintf(Keypadvalue, "%1X", chip8.getKey(i)); // Hexadecimal ABCD
        Keypadtext += Keypadvalue;
        // Vtext += "\n";
        SDL_Surface* surfaceMessageKeypad = TTF_RenderText_Solid(font, Keypadtext.c_str(), White);
        SDL_Texture* MessageKeypad = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageKeypad);
        
        SDL_Rect Message_rectKeypad;
        Message_rectKeypad.x = xDisplay + 3.2*(SCREEN_WIDTH-xDisplay)/4;
        Message_rectKeypad.y = yDisplay + i*(SCREEN_HEIGHT-yDisplay)/16;
        Message_rectKeypad.w = surfaceMessageKeypad->w;
        Message_rectKeypad.h = surfaceMessageKeypad->h;
        
        SDL_RenderCopy((*this).renderer, MessageKeypad, NULL, &Message_rectKeypad);
        SDL_FreeSurface(surfaceMessageKeypad);
        SDL_DestroyTexture(MessageKeypad);
    }
}


void Screen::draw(uint8_t display[]){
    int spriteW = 0.5*SCREEN_WIDTH/64;
    int spriteH = 0.5*SCREEN_HEIGHT/32;
    
    for(int x=0; x < 64; x++){
        for(int y=0; y < 32; y++){
            SDL_SetRenderDrawColor((*this).renderer, display[y*64+x]*255, display[y*64+x]*255, display[y*64+x]*255, 0);
            /* Optimized Way */
            SDL_Rect fillRect = {x*spriteW, y*spriteH, spriteW, spriteH}; 
            SDL_RenderFillRect((*this).renderer, &fillRect);
            
            /* Non Optimized Way */
            // for(int xoff=0; xoff < spriteW; xoff++){
            //     for(int yoff=0; yoff < spriteH; yoff++){
            //         SDL_RenderDrawPoint((*this).renderer,x*spriteW + xoff, y*spriteH + yoff);
            //     }
            // }
        }
    }
}


void Screen::drawKeyboard(Chip8 chip8){
    int spriteW = 0.5*SCREEN_WIDTH/64;
    int spriteH = 0.5*SCREEN_HEIGHT/32;
    int xDisplay = 0.5*SCREEN_WIDTH;
    int yDisplay = 0.5*SCREEN_HEIGHT;
    
    TTF_Font* font = TTF_OpenFont("./PressStart2P-Regular.ttf", spriteW);
    // SDL_Color White = {123, 125, 201};

    // std::string SPtext = "";
    // SPtext += "SP:";
    // char SPvalue[8];
    // sprintf(SPvalue, "%02X", chip8.getSp()); // Hexadecimal ABCD
    // SPtext += SPvalue;
    const char keys[16] = {'1','2','3','C',
                            '4','5','6','D',
                            '7','8','9','E',
                            'A','0','B','F'};

                            
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            char text[2] = {keys[i + j*4],'\0'};

            int hexValue;
            if (keys[i + j * 4] >= '0' && keys[i + j * 4] <= '9') {
                hexValue = keys[i + j * 4] - '0';   
            } else {
                hexValue = keys[i + j * 4] - 'A' + 10;
            }

            int keyPressed = chip8.getKey(hexValue);
            SDL_Color White = {255 - keyPressed*(255-123), 255 - keyPressed*(255-125), 255 - keyPressed*(255-201)};

            SDL_Surface* surfaceMessageSP = TTF_RenderText_Solid(font, text, White);
            SDL_Texture* MessageSP = SDL_CreateTextureFromSurface((*this).renderer, surfaceMessageSP);
                
            SDL_Rect Message_rectSP;
            Message_rectSP.x = i*(SCREEN_WIDTH-xDisplay)/12 + (SCREEN_WIDTH-xDisplay)/3;
            Message_rectSP.y = j*(SCREEN_HEIGHT-yDisplay)/5 + (SCREEN_HEIGHT-yDisplay)/5 + yDisplay;
            // Message_rectSP.w = surfaceMessageSP->w;
            // Message_rectSP.h = surfaceMessageSP->h;
            Message_rectSP.w = (SCREEN_WIDTH-xDisplay)/12;
            Message_rectSP.h = (SCREEN_HEIGHT-yDisplay)/12;
                
            SDL_RenderCopy((*this).renderer, MessageSP, NULL, &Message_rectSP);
            SDL_FreeSurface(surfaceMessageSP);
            SDL_DestroyTexture(MessageSP);
        }
    }
}
