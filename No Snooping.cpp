/*
UI COMPONENTS
- button
- text [ok]
- video player
*/

#include <iostream>
#include <windows.h>
#include <vector>

#include "include/Button.hpp"
#include "include/Textures.hpp"
#include "include/Font_Renderer.hpp"
#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

int main(int argc, char* argv[]) {

    // Init SDL
    // HWND hWnd = GetConsoleWindow();
    // ShowWindow(hWnd, SW_HIDE);
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("No Snooping", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Frame limiting
    const int FPS = 60;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 frStart = 0;
    int frTime = 0;

    // Snooper display
    std::vector<std::string> spFiles;
    SDL_Rect spPlaceRect = {200, 0, 600, 600};

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

        // Render UI
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &spPlaceRect);
        RenderTextCentered(renderer, {0, 0, 0}, spPlaceRect, "No file currently open.", "fonts/verdana.ttf", 30);
        /*
        scrollbar with multiple buttons how
        Display list of failed logins, thumbnail
        When open, display big, and info
        When nothing is open (before anything is open), show some text saying u need to open
        */

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