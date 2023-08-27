#include <iostream>
#include <unordered_map>
#include <vector>

#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_ttf.h>
#include <SDL2/include/SDL2/SDL_image.h>

#pragma once

// Character STRUCT
struct SDL_CHAR {
    SDL_Texture* tex;
    int w, h;
    int render(SDL_Renderer* renderer, SDL_Color color, int x, int y) {
        SDL_Rect rect = {x, y, w, h};
        SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        return x + w;
    }
};

// Rendering from cached characters
std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<char, SDL_CHAR>>> charCache;
void RenderTextFromCache(SDL_Renderer* renderer, SDL_Color color, int x, int y, std::string text, std::string font, int size) {
    for (char c: text) {
        x = charCache[font][size][c].render(renderer, color, x, y);
    }
}

// Load font with size
std::string loadChars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()?.,[]{};:'\"/<>+=-_";
void LoadFontWithSize(SDL_Renderer* renderer, std::string font, int size) {
    TTF_Font* fontObj = TTF_OpenFont(font.c_str(), size);
    for (char c: loadChars) {
        std::string cStr(1, c);
        SDL_Surface* surf = TTF_RenderText_Blended(fontObj, cStr.c_str(), {255, 255, 255});
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        charCache[font][size][c] = {tex, surf->w, surf->h};
        SDL_FreeSurface(surf);
    }
}

// Main text render function
void RenderText(SDL_Renderer* renderer, SDL_Color color, int x, int y, std::string text, std::string font, int size) {

    // Check if font has already been loaded, load font with size
    if (charCache.find(font) == charCache.end() || charCache[font].find(size) == charCache[font].end()) {
        LoadFontWithSize(renderer, font, size);
    }

    // Finally render text
    RenderTextFromCache(renderer, color, x, y, text, font, size);
}

// Get dimensions of text to be rendered
std::pair<int, int> GetTextDimensions(SDL_Renderer* renderer, std::string text, std::string font, int size) {

    // Check if font has already been loaded, load font with size
    if (charCache.find(font) == charCache.end() || charCache[font].find(size) == charCache[font].end()) {
        LoadFontWithSize(renderer, font, size);
    }

    // Calculations
    int textW = 0, textH;
    for (char c: text) {
        textW += charCache[font][size][c].w;
        textH = charCache[font][size][c].h;
    }
    return {textW, textH};
}