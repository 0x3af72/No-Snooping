#include <iostream>
#include <functional>

#include "Textures.hpp"
#include "Font_Renderer.hpp"
#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

#pragma once

struct SDL_BUTTON {
    int x, y, w, h;
    std::function<void()> func;
    SDL_Color color, tColor;
    SDL_Texture* tex;
    bool fromTex;
    std::string text, font;
    int size;
};

SDL_BUTTON CreateButtonFromColor(int x, int y, int w, int h, std::function<void()> func, SDL_Color color, SDL_Color tColor, std::string text, std::string font, int size) {
    return {x, y, w, h, func, color, tColor, NULL, false, text, font, size};
}

SDL_BUTTON CreateButtonFromTexture(int x, int y, int w, int h, std::function<void()> func, SDL_Renderer* renderer, std::string file) {
    return {x, y, w, h, func, {0, 0, 0}, {0, 0, 0}, LoadTexture(renderer, file), true};
}

bool UpdateButton(SDL_BUTTON& button, SDL_Renderer* renderer, SDL_Rect mRect, bool mouseUp) {

    // Check if hovers, button clicked
    SDL_Rect bRect = {button.x, button.y, button.w, button.h};
    bool hovers = SDL_HasIntersection(&bRect, &mRect);
    bool clicked = hovers && mouseUp;
    SDL_Color nColor = button.color;
    if (hovers) {
        nColor.r = std::max(0, nColor.r - 20);
        nColor.g = std::max(0, nColor.g - 20);
        nColor.b = std::max(0, nColor.b - 20);
    }

    // Display
    float disX = button.x, disY = button.y;
    float disW = button.w, disH = button.h;
    float disTextSz = button.size;
    if (hovers) {
        disTextSz *= 1.05;
        disX -= disW * 0.025; disY -= disH * 0.025;
        disW *= 1.05; disH *= 1.05;
    }
    SDL_Rect disRect = {disX, disY, disW, disH};
    if (button.fromTex) {
        SDL_RenderCopy(renderer, button.tex, NULL, &disRect);
    } else {
        SDL_SetRenderDrawColor(renderer, nColor.r, nColor.g, nColor.b, nColor.a);
        SDL_RenderFillRect(renderer, &disRect);
        SDL_SetRenderDrawColor(renderer, button.tColor.r, button.tColor.g, button.tColor.b, nColor.a);
        SDL_RenderDrawRect(renderer, &disRect);
        RenderTextCentered(renderer, button.tColor, disRect, button.text, button.font, disTextSz);
    }

    // Invoke function
    if (clicked) {
        button.func();
    }
    return clicked;
}