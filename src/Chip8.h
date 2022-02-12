//
// Created by Noah Schonhorn on 11/11/21.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <cstdint>
#include <array>
#include <stack>
#include <random>
#include <iostream>
#include <chrono>
#include "SDL.h"

class Chip8
{
private:
    const static int SCREEN_WIDTH = 64;
    const static int SCREEN_HEIGHT = 32;

    SDL_Renderer* renderer;
    SDL_Texture* renderTex;
    bool createEmptyTexture();

    uint8_t memory[0x1000];
    uint16_t pc;
    uint16_t index_reg;
    uint8_t variable_reg[16];
    std::stack<uint16_t> stack;

    bool displayArr[SCREEN_WIDTH][SCREEN_HEIGHT];
    uint8_t xCoord, yCoord, spriteData, currentPixel;

    uint8_t keypad[16];

    uint8_t delayTimer;
    uint8_t soundTimer;

    std::mt19937 gen;
    std::uniform_int_distribution<uint8_t> dist;

    SDL_Rect* destRect;

public:
    Chip8(SDL_Renderer** renderer, int screenWidth, int screenHeight, SDL_Rect* destRect);
    ~Chip8();

    void cpu_cycle();
    bool load_rom(std::string path);
    void get_input(SDL_Event* e);

    void realRender();
};


#endif //CHIP8_CHIP8_H
