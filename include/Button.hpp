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
    SDL_Color color, text_color;
    SDL_Texture* tex;
    bool from_tex;
    std::string text, font;
    int size;
};

SDL_BUTTON Button_From_Color(int x, int y, int w, int h, std::function<void()> func, SDL_Color color, SDL_Color text_color, std::string text, std::string font, int size) {
    return {x, y, w, h, func, color, text_color, NULL, false, text, font, size};
}

SDL_BUTTON Button_From_Texture(int x, int y, int w, int h, std::function<void()> func, SDL_Renderer* renderer, std::string file) {
    return {x, y, w, h, func, {0, 0, 0}, {0, 0, 0}, Load_Texture(renderer, file), true};
}

bool Update_And_Render_Button(SDL_BUTTON& button, SDL_Renderer* renderer, SDL_Rect mRect, bool mouseUp) {

    // Check if hovers, button clicked
    SDL_Rect bRect = {button.x, button.y, button.w, button.h};
    bool hovers = SDL_HasIntersection(&bRect, &mRect);
    bool clicked = hovers && mouseUp;

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
    if (button.from_tex) {
        SDL_RenderCopy(renderer, button.tex, NULL, &disRect);
    } else {
        SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, 255);
        SDL_RenderFillRect(renderer, &disRect);
        auto [textW, textH] = GetTextDimensions(renderer, button.text, button.font, disTextSz);
        disX += (disW - textW) / 2;
        disY += (disH - textH) / 2;
        RenderText(renderer, button.text_color, disX, disY, button.text, button.font, disTextSz);
    }

    // Invoke function
    if (clicked) {
        button.func();
    }
    return clicked;
}