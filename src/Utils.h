//
// Created by Noah Schonhorn on 4/22/21.
//

#ifndef CHIP8_UTILS_H
#define CHIP8_UTILS_H

#include "SDL.h"

bool init(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight);
void close(SDL_Window** window, SDL_Renderer** renderer);

#endif //CHIP8_UTILS_H
