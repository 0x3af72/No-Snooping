/*
UI COMPONENTS
- button
- text [ok]
- video player
*/

#include <iostream>
#include <windows.h>

#include "include/Button.hpp"
#include "include/Textures.hpp"
#include "include/Font_Renderer.hpp"
#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

int main(int argc, char* argv[]) {

    // Init SDL
    // FreeConsole();
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("No Snooping", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Frame limiting
    const int FPS = 60;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 frStart = 0;
    int frTime = 0;

    SDL_BUTTON test_btn = Button_From_Color(100, 100, 200, 150, [](){std::cout << "clicked\n";}, {255, 0, 0}, {0, 0, 0}, "hello", "fonts/verdana.ttf", 10);

    // Mainloop
    bool run = true;
    while (run) {

        // Get mouse data
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Rect mRect = {mx, my, 1, 1};

        // Handle events
        bool LMU = false, RMU = false;
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                run = false;
            } else if (ev.type == SDL_MOUSEBUTTONUP) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    LMU = true;
                } else if (ev.button.button == SDL_BUTTON_RIGHT) {
                    RMU = true;
                }
            }
        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Hmm
        RenderText(renderer, {255, 255, 255}, 0, 0, "helloworld", "fonts/verdana.ttf", 200);
        Update_And_Render_Button(test_btn, renderer, mRect, LMU);

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