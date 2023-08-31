#include <iostream>
#include <vector>

#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

#pragma once

// How do we cut off rendering at some point? (half in half out of the scrollbar)
struct SDL_VERTICAL_SCROLLBAR {
    int x, y, w, h;
    std::vector<int> elements; // heights
    int vHeight; // Starts at 0, ends at (total elem height - h)
    SDL_Color color;
    SDL_Rect sRect;
};

SDL_VERTICAL_SCROLLBAR CreateVerticalScrollbar(int x, int y, int w, int h, std::vector<int> elements, SDL_Color color) {
    return {x, y, w, h, elements, 0, color};
}

std::vector<int> UpdateVerticalScrollbar(SDL_VERTICAL_SCROLLBAR& scroll, SDL_Renderer* renderer, SDL_Rect mRect, int scrVel) {

    // Update viewheight and positions
    std::vector<int> oPos;
    int cur = 0;
    int tHeight;
    for (int h: scroll.elements) {
        tHeight += h;
        oPos.push_back(cur);
        cur += h;
    }
    scroll.sRect = {scroll.x, scroll.y, scroll.w, scroll.h};
    if (SDL_HasIntersection(&scroll.sRect, &mRect)) {
        scroll.vHeight += scrVel;
    }
    scroll.vHeight = std::min(tHeight - scroll.h, scroll.vHeight);
    scroll.vHeight = std::max(0, scroll.vHeight);
    std::vector<int> uPos = oPos;
    for (int& p: uPos) {
        p -= scroll.vHeight;
    }

    // Display
    SDL_SetRenderDrawColor(renderer, scroll.color.r, scroll.color.g, scroll.color.b, scroll.color.a);
    SDL_RenderDrawRect(renderer, &scroll.sRect);

    return uPos;
}