#include <iostream>

#include <SDL2/include/SDL2/SDL.h>

#pragma once

SDL_Rect CenterRect(SDL_Rect in, SDL_Rect to) {
    to.x = in.x + (in.w - to.w) / 2;
    to.y = in.y + (in.h - to.h) / 2;
    return to;
}