#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

class Screen {
    public:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
    
    Screen(int width, int height){
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
    }
    
    void initialize();
    void draw(uint8_t display[]);

        
};

#endif
