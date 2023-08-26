/*
UI COMPONENTS
- button
- text (i need to use the text renderer or rewrite it/adapt it)
- video player
*/

#include <iostream>

#include "include/Textures.hpp"
#include "include/Font_Renderer.hpp"
#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

int main(int argc, char* argv[]) {

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("No Snooping", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Frame limiting
    const int FPS = 60;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 frStart = 0;
    int frTime = 0;

    // Mainloop
    bool run = true;
    while (run) {

        // Handle events
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            
            // QUIT
            if (ev.type == SDL_QUIT) {
                run = false;
            }

        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Hmm
        RenderText(renderer, {255, 255, 255}, 0, 0, "helloworld", "fonts/verdana.ttf", 200);

        // Present renderer
        SDL_RenderPresent(renderer);

        // Frame limiting
        frTime = SDL_GetTicks() - frStart;
        if (FRAME_DELAY > frTime){
            SDL_Delay(FRAME_DELAY - frTime);
        }
        // std::cout << FRAME_DELAY - frTime << "\n";
        frStart = SDL_GetTicks();

    }

    return 0;
}