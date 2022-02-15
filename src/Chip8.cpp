//
// Created by Noah Schonhorn on 11/11/21.
//

#include "Chip8.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

Chip8::Chip8(SDL_Renderer** renderer, int screenWidth, int screenHeight, SDL_Rect* destRect) : gen(std::chrono::system_clock::now().time_since_epoch().count())
{
    std::array<uint8_t, 5 * 16> fonts =
            {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    for (int i = 0; i < fonts.size(); ++i)
    {
        uint8_t offset = 0x050;
        memory[i + offset] = fonts[i];
    }

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            displayArr[x][y] = false;
        }
    }

    for (int i = 0; i < 16; ++i)
    {
        keypad[i] = 0;
    }

    dist = std::uniform_int_distribution<uint8_t>(0, 255);

    pc = 0x200u;

    this->renderer = *renderer;

    this->destRect = destRect;

    createEmptyTexture();
}

void Chip8::get_input(SDL_Event *e)
{
    switch (e->type)
    {
        case SDL_KEYDOWN:
            switch (e->key.keysym.sym)
            {
                case SDLK_x:
                    keypad[0] = 1;
                    break;
                case SDLK_1:
                    keypad[1] = 1;
                    break;
                case SDLK_2:
                    keypad[2] = 1;
                    break;
                case SDLK_3:
                    keypad[3] = 1;
                    break;
                case SDLK_q:
                    keypad[4] = 1;
                    break;
                case SDLK_w:
                    keypad[5] = 1;
                    break;
                case SDLK_e:
                    keypad[6] = 1;
                    break;
                case SDLK_a:
                    keypad[7] = 1;
                    break;
                case SDLK_s:
                    keypad[8] = 1;
                    break;
                case SDLK_d:
                    keypad[9] = 1;
                    break;
                case SDLK_z:
                    keypad[10] = 1;
                    break;
                case SDLK_c:
                    keypad[11] = 1;
                    break;
                case SDLK_4:
                    keypad[12] = 1;
                    break;
                case SDLK_r:
                    keypad[13] = 1;
                    break;
                case SDLK_f:
                    keypad[14] = 1;
                    break;
                case SDLK_v:
                    keypad[15] = 1;
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (e->key.keysym.sym)
            {
                case SDLK_x:
                    keypad[0] = 0;
                    break;
                case SDLK_1:
                    keypad[1] = 0;
                    break;
                case SDLK_2:
                    keypad[2] = 0;
                    break;
                case SDLK_3:
                    keypad[3] = 0;
                    break;
                case SDLK_q:
                    keypad[4] = 0;
                    break;
                case SDLK_w:
                    keypad[5] = 0;
                    break;
                case SDLK_e:
                    keypad[6] = 0;
                    break;
                case SDLK_a:
                    keypad[7] = 0;
                    break;
                case SDLK_s:
                    keypad[8] = 0;
                    break;
                case SDLK_d:
                    keypad[9] = 0;
                    break;
                case SDLK_z:
                    keypad[10] = 0;
                    break;
                case SDLK_c:
                    keypad[11] = 0;
                    break;
                case SDLK_4:
                    keypad[12] = 0;
                    break;
                case SDLK_r:
                    keypad[13] = 0;
                    break;
                case SDLK_f:
                    keypad[14] = 0;
                    break;
                case SDLK_v:
                    keypad[15] = 0;
                    break;
            }
            break;
    }
}

void Chip8::cpu_cycle()
{
    uint16_t opcode = memory[pc];
    opcode = opcode << 8;
    opcode = opcode | memory[pc + 1];
    pc += 2;

    uint8_t firstNibble = (opcode & 0xF000) >> 12;
    uint8_t secondNibble = (opcode & 0x0F00) >> 8;
    uint8_t thirdNibble = (opcode & 0x00F0) >> 4;
    uint8_t fourthNibble = (opcode & 0x000F);

    switch (firstNibble)
    {
        case 0x0:
            switch (fourthNibble)
            {
                case 0x0:
                    //00E0: Clear screen
                    for (int x = 0; x < SCREEN_WIDTH; ++x)
                    {
                        for (int y = 0; y < SCREEN_HEIGHT; ++y)
                        {
                            displayArr[x][y] = false;
                        }
                    }
                    break;
                case 0xE:
                    //00EE: Return from subroutine
                    pc = stack.top();
                    stack.pop();
                    break;
                default:
                    break;
            }
            break;
        case 0x1:
            //1NNN: Jump to address NNN
            pc = (opcode & 0x0FFF);
            break;
        case 0x2:
            //2NNN: execute subroutine at NNN
            stack.push(pc);
            pc = (opcode & 0x0FFF);
            break;
        case 0x3:
            //3XNN: Skip next instr. if VX == NN
            if (variable_reg[secondNibble] == (opcode & 0x00FFu))
            {
                pc += 2;
            }
            break;
        case 0x4:
            //4XNN: Skip next instr. if VX != NN
            if (variable_reg[secondNibble] != (opcode & 0x00FFu))
            {
                pc += 2;
            }
            break;
        case 0x5:
            //5XY0: Skip next instr. if VX == VY
            if (variable_reg[secondNibble] == variable_reg[thirdNibble])
            {
                pc += 2;
            }
            break;
        case 0x6:
            //6XNN: Store NN in register VX
            variable_reg[secondNibble] = (opcode & 0x00FF);
            break;
        case 0x7:
            //7XNN: Add value NN to register VX
            variable_reg[secondNibble] += (opcode & 0x00FF);
            break;
        case 0x8:
            switch (fourthNibble)
            {
                case 0x0:
                    //8XY0: Set VX to VY
                    variable_reg[secondNibble] = variable_reg[thirdNibble];
                    break;
                case 0x1:
                    //8XY1: Binary OR
                    variable_reg[secondNibble] |= variable_reg[thirdNibble];
                    break;
                case 0x2:
                    //8XY2: Binary AND
                    variable_reg[secondNibble] &= variable_reg[thirdNibble];
                    break;
                case 0x3:
                    //8XY3: Binary XOR
                    variable_reg[secondNibble] ^= variable_reg[thirdNibble];
                    break;
                case 0x4:
                    //8XY4: Add w/ overflow
                    if (variable_reg[secondNibble] + variable_reg[thirdNibble] > 0xFF)
                    {
                        variable_reg[0xF] = 1;
                    }
                    else
                    {
                        variable_reg[0xF] = 0;
                    }

                    variable_reg[secondNibble] += variable_reg[thirdNibble];
                    break;
                case 0x5:
                    //8XY5: Subtract VY from VX w/ overflow
                    if (variable_reg[secondNibble] > variable_reg[thirdNibble])
                    {
                        variable_reg[0xF] = 1;
                    }
                    else
                    {
                        variable_reg[0xF] = 0;
                    }

                    variable_reg[secondNibble] -= variable_reg[thirdNibble];
                    break;
                case 0x6:
                    //8XY6: Store VY shifted right once in VX, set VF to LSB
                    variable_reg[0xF] = variable_reg[secondNibble] & 0x1;
                    variable_reg[secondNibble] = variable_reg[thirdNibble];
                    variable_reg[secondNibble] >>= 1;
                    break;
                case 0x7:
                    //8XY7: Set VX to VY - VX w/ overflow
                    if (variable_reg[thirdNibble] > variable_reg[secondNibble])
                    {
                        variable_reg[0xF] = 1;
                    }
                    else
                    {
                        variable_reg[0xF] = 0;
                    }

                    variable_reg[secondNibble] = variable_reg[thirdNibble] - variable_reg[secondNibble];
                    break;
                case 0xE:
                    //8XYE: Store VY shifted left once in VX, set VF to MSB
                    variable_reg[0xF] = (variable_reg[secondNibble] >> 7);
                    variable_reg[secondNibble] = variable_reg[thirdNibble];
                    variable_reg[secondNibble] <<= 1;
                    break;
                default:
                    break;
            }
            break;
        case 0x9:
            //9XY0: Skip next instr. if VX != VY
            if (variable_reg[secondNibble] != variable_reg[thirdNibble])
            {
                pc += 2;
            }
            break;
        case 0xA:
            //ANNN: Set index register to NNN
            index_reg = (opcode & 0x0FFF);
            break;
        case 0xB:
            //BNNN: Jump w/ offset
            pc = (opcode & 0x0FFF) + variable_reg[0];
            break;
        case 0xC:
            //CXNN: Generate random number, AND with NN, and store in VX
            variable_reg[secondNibble] = (dist(gen) & (opcode & 0x00FF));
            break;
        case 0xD:
            //DXYN: Draw display
            xCoord = variable_reg[secondNibble] % 64;
            yCoord = variable_reg[thirdNibble] % 32;

            variable_reg[0xF] = 0;
            for (int row = 0; row < fourthNibble && yCoord + row < SCREEN_HEIGHT; ++row)
            {
                spriteData = memory[index_reg + row];
                for (int col = 0; col <= 8 && xCoord + col < SCREEN_WIDTH; ++col)
                {
                    currentPixel = ((spriteData >> (8 - col)) & 0x01);

                    if (currentPixel == 1)
                    {
                        if (displayArr[xCoord + col][yCoord + row])
                        {
                            variable_reg[0xF] = 1;
                        }
                    }

                    displayArr[xCoord + col][yCoord + row] = !displayArr[xCoord + col][yCoord + row] != currentPixel != 1;
                }
            }
            realRender();
            break;
        case 0xE:
            switch (thirdNibble)
            {
                case 0x9:
                    if (keypad[variable_reg[secondNibble]])
                    {
                        pc += 2;
                    }
                    break;
                case 0xA:
                    if (!keypad[variable_reg[secondNibble]])
                    {
                        pc += 2;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 0xF:
            switch (fourthNibble)
            {
                case 0x7:
                    //FX07: Set VX to value of delay timer
                    variable_reg[secondNibble] = delayTimer;
                    break;
                case 0x5:
                    switch (thirdNibble)
                    {
                        case 0x1:
                            //FX15: Set delay timer to value in VX
                            delayTimer = variable_reg[secondNibble];
                            break;
                        case 0x5:
                            //FX55: Store values from V0 to VX inclusive in memory starting at index register
                            for (int i = 0; i <= secondNibble; ++i)
                            {
                                memory[index_reg + i] = variable_reg[i];
                            }
                            break;
                        case 0x6:
                            //FX65: Fill index registers with values stored in memory starting at index register
                            for (int i = 0; i <= secondNibble; ++i)
                            {
                                variable_reg[i] = memory[index_reg + i];
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case 0x8:
                    //FX18: Set sound timer to value in VX
                    soundTimer = variable_reg[secondNibble];
                    break;
                case 0xA:
                    //FX0A: Wait for key press
                    if (keypad[0])
                    {
                        variable_reg[secondNibble] = 0;
                    }
                    else if (keypad[1])
                    {
                        variable_reg[secondNibble] = 1;
                    }
                    else if (keypad[2])
                    {
                        variable_reg[secondNibble] = 2;
                    }
                    else if (keypad[3])
                    {
                        variable_reg[secondNibble] = 3;
                    }
                    else if (keypad[4])
                    {
                        variable_reg[secondNibble] = 4;
                    }
                    else if (keypad[5])
                    {
                        variable_reg[secondNibble] = 5;
                    }
                    else if (keypad[6])
                    {
                        variable_reg[secondNibble] = 6;
                    }
                    else if (keypad[7])
                    {
                        variable_reg[secondNibble] = 7;
                    }
                    else if (keypad[8])
                    {
                        variable_reg[secondNibble] = 8;
                    }
                    else if (keypad[9])
                    {
                        variable_reg[secondNibble] = 9;
                    }
                    else if (keypad[10])
                    {
                        variable_reg[secondNibble] = 10;
                    }
                    else if (keypad[11])
                    {
                        variable_reg[secondNibble] = 11;
                    }
                    else if (keypad[12])
                    {
                        variable_reg[secondNibble] = 12;
                    }
                    else if (keypad[13])
                    {
                        variable_reg[secondNibble] = 13;
                    }
                    else if (keypad[14])
                    {
                        variable_reg[secondNibble] = 14;
                    }
                    else if (keypad[15])
                    {
                        variable_reg[secondNibble] = 15;
                    }
                    else
                    {
                        pc -= 2;
                    }
                    break;
                case 0xE:
                    //FX1E: Add to index
                    if (index_reg + variable_reg[secondNibble] > 0x1000)
                    {
                        variable_reg[0xF] = 1;
                    }

                    index_reg += variable_reg[secondNibble];
                    break;
                case 0x9:
                    //FX29: Set index register to addr of digit stored in memory
                    index_reg = 0x050 + (variable_reg[secondNibble] * 5);
                    break;
                case 0x3:
                    //FX33: Binary to decimal
                    memory[index_reg] = variable_reg[secondNibble] % 1000 / 100;
                    memory[index_reg + 1]  = variable_reg[secondNibble] % 100 / 10;
                    memory[index_reg + 2] = variable_reg[secondNibble] % 10;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if (delayTimer > 0)
    {
        delayTimer--;
    }

    if (soundTimer > 0)
    {
        soundTimer--;
    }
}

bool Chip8::createEmptyTexture()
{
    renderTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (renderTex == nullptr)
    {
        printf("Unable to create blank texture! SDL Error:%s\n", SDL_GetError());
    }
    return renderTex != nullptr;
}

void Chip8::realRender()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, renderTex);

    SDL_RenderFillRect(renderer, destRect);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            if (displayArr[x][y])
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, renderTex, nullptr, destRect);
    SDL_RenderPresent(renderer);
}

bool Chip8::loadROM(const std::string& path)
{
    std::ifstream rom_file(path, std::ios::binary | std::ios::ate);

    if (rom_file.is_open())
    {
        std::streampos size = rom_file.tellg();
        char* buffer = new char[size];

        rom_file.seekg(0, std::ios::beg);
        rom_file.read(buffer, size);
        rom_file.close();

        for (int i = 0; i < size; ++i)
        {
            memory[0x200 + i] = buffer[i];
        }

        delete[] buffer;
    }
    else
    {
        return false;
    }

    return true;
}

Chip8::~Chip8()
{
    renderer = nullptr;
    SDL_DestroyTexture(renderTex);
    renderTex = nullptr;
}
