#include <iostream>
#include <unordered_map>

#include <SDL2/include/SDL2/SDL.h>
#include <SDL2/include/SDL2/SDL_image.h>

#pragma once

// Function to load a texture and cache it
std::unordered_map<std::string, SDL_Texture*> texCache;
SDL_Texture* Load_Texture(SDL_Renderer* renderer, std::string file){

    // Check if texture already loaded
    if (texCache.find(file) != texCache.end()){
        return texCache[file];
    }

    // Load texture
    SDL_Surface* surf = IMG_Load(file.c_str());
    if (!surf){
        std::cout << "Unable to load image: " << SDL_GetError() << "\n";
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    // Cache texture
    texCache[file] = tex;
    return tex;
}