#include <iostream>
#include <windows.h>
#include <vector>
#include <filesystem>
#include <tchar.h>
#include <algorithm>
#include <time.h>

#include "include/Button.hpp"
#include "include/Textures.hpp"
#include "include/Scrollbar.hpp"
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
    SDL_VERTICAL_SCROLLBAR spScroll = CreateVerticalScrollbar(0, 0, 200, 600, {}, {255, 255, 255});
    std::vector<std::pair<int, SDL_BUTTON>> spButtons;
    SDL_Rect spTexRect;
    SDL_Texture* spTex = NULL;

    // Mainloop
    bool run = true;
    long ticks = 0;
    while (run) {

        // Get mouse data
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Rect mRect = {mx, my, 1, 1};

        // Handle events
        ticks++;
        bool LMU = false, RMU = false;
        int spScrVel = 0;
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
            } else if (ev.type == SDL_MOUSEWHEEL) {
                spScrVel = -ev.wheel.y;
            }
        }

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render UI
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &spPlaceRect);
        if (!spTex) {
            RenderTextCentered(renderer, {0, 0, 0}, spPlaceRect, "No file currently open.", "fonts/verdana.ttf", 30);
        }
        if (ticks % 60 == 0) {
            spFiles.clear();
            for (auto entry: std::filesystem::directory_iterator("Snoopers")) {
                if (!entry.is_regular_file()) {
                    continue;
                }
                spFiles.push_back(entry.path().filename().string());
            }
            spButtons.clear();
            for (std::string file: spFiles) {

                if (file.substr(file.size() - 3, 3) == "avi") {
                    continue;
                }
                
                WIN32_FIND_DATA scData;
                FindFirstFile(("Snoopers/" + file).c_str(), &scData);
                TCHAR tcDate[80];
                SYSTEMTIME ct, utcSysTime;
                FileTimeToSystemTime(&scData.ftCreationTime, &ct);
                FILETIME utcFTime;
                SystemTimeToFileTime(&ct, &utcFTime);
                FileTimeToSystemTime(&utcFTime, &utcSysTime);
                SystemTimeToTzSpecificLocalTime(NULL, &utcSysTime, &ct);

                GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &ct, NULL, tcDate, _countof(tcDate));
                _tcscat_s(tcDate, sizeof(tcDate), TEXT(" "));
                GetTimeFormat(LOCALE_USER_DEFAULT, 0, &ct, NULL, tcDate + strlen(tcDate), _countof(tcDate) - strlen(tcDate));
                std::string time = tcDate;
                int timeI = ct.wYear * 31536000 + ct.wMonth * 2592000 + ct.wDay * 86400 + ct.wHour * 3600 + ct.wMinute * 60 + ct.wSecond * 1;

                spButtons.push_back({timeI, CreateButtonFromColor(0, 0, 200, 60, [&renderer, &spTex, &spTexRect, file]() {
                    spTex = LoadTexture(renderer, "Snoopers/" + file);
                    int tW, tH;
                    SDL_QueryTexture(spTex, NULL, NULL, &tW, &tH);
                    if (tW > 600) {
                        spTexRect = {200, 0, 600, 600 / (float) tW * tH};
                    } else if (tH > 600) {
                        spTexRect = {200, 0, 600 / (float) tH * tW, 600};
                    } else {
                        spTexRect = {200, 0, tW, tH};
                    }
                }, {200, 200, 200}, {0, 0, 0}, time, "fonts/verdana.ttf", 10)});
            }
            std::sort(spButtons.begin(), spButtons.end(), [](std::pair<int, SDL_BUTTON> f, std::pair<int, SDL_BUTTON> s) {
                return f.first > s.first;
            });
            spScroll.elements = std::vector<int>(spButtons.size(), 60);
        }
        int i = 0;
        SDL_RenderSetViewport(renderer, &spScroll.sRect);
        for (int y: UpdateVerticalScrollbar(spScroll, renderer, mRect, spScrVel * 20)) {
            spButtons[i].second.y = y;
            UpdateButton(spButtons[i].second, renderer, mRect, LMU);
            i++;
        }
        SDL_RenderSetViewport(renderer, NULL);
        if (spTex) {
            SDL_Rect spTexRectC = CenterRect(spPlaceRect, spTexRect);
            SDL_RenderCopy(renderer, spTex, NULL, &spTexRectC);
            RenderTextCentered(renderer, {255, 0, 0}, {200, 0, 600, 100}, "SNOOPER DETECTED!", "fonts/verdana.ttf", 30);
        }

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