//
// Created by Noah Schonhorn on 4/22/21.
//

#include "Utils.h"

bool init(SDL_Window **window, SDL_Renderer **renderer, int screenWidth, int screenHeight)
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set mTexture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
        {
            printf("Warning: OpenGL not being used!");
        }

        //Create window
        *window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                   screenHeight, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
            if (renderer == nullptr)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
//                int imgFlags = IMG_INIT_PNG;
//                if (!(IMG_Init(imgFlags) & imgFlags))
//                {
//                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
//                    success = false;
//                }
            }
        }
    }

    return success;
}

void close(SDL_Window** window, SDL_Renderer** renderer)
{
    *window = nullptr;
    *renderer = nullptr;

    SDL_Quit();
    //IMG_Quit();
}