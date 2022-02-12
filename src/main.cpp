#include <iostream>
#include <thread>
#include "Chip8.h"
#include "Utils.h"
#include "SDL.h"

int main(int argc, char **argv)
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    const int SCALE_FACTOR = 14;
    const int DISPLAY_WIDTH = 64;
    const int DISPLAY_HEIGHT = 32;

    int cyclesPerSecond = 300;
    int screenTicksPerFrame = 1000 / cyclesPerSecond;
    int countedFrames = 0;

    SDL_Rect destRec= {0, 0, DISPLAY_WIDTH * SCALE_FACTOR, DISPLAY_HEIGHT * SCALE_FACTOR};

    SDL_Event e;

    std::string romPath;

    if (argc < 2)
    {
        std::cout << "No ROM file given, exiting..." << std::endl;
        return -1;
    }
    else
    {
        romPath = argv[1];
    }

    if (!init(&window, &renderer, DISPLAY_WIDTH * SCALE_FACTOR, DISPLAY_HEIGHT * SCALE_FACTOR))
    {
        printf("Failed to intialize!\n");
    }
    else
    {
        Chip8 cpu(&renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT, &destRec);

        bool quit = false;
        bool pauseExec = false;

        Uint32 startTime = SDL_GetTicks();

        if (!cpu.load_rom(romPath))
        {
            std::cout << "ROM file missing, exiting..." << std::endl;
            return -1;
        }

        while (!quit)
        {
            Uint32 cycleCapTimer = SDL_GetTicks();

            while (SDL_PollEvent(&e) != 0)
            {

                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_p:
                            pauseExec = !pauseExec;
                            break;
                        case SDLK_SPACE:
                            if (pauseExec)
                            {
                                cpu.cpu_cycle();
                            }
                            break;
                    }
                }

                cpu.get_input(&e);
            }

            if (!pauseExec)
            {
                cpu.cpu_cycle();
            }

            float avgFPS = countedFrames / ((SDL_GetTicks() - startTime) / 1000.f);
            if (avgFPS > 2000000)
            {
                avgFPS = 0;
            }

            int frameTicks = cycleCapTimer - SDL_GetTicks();

            if (frameTicks < screenTicksPerFrame)
            {
                SDL_Delay(screenTicksPerFrame - frameTicks);
                std::string windowTitle = "Chip8 | FPS: " + std::to_string((int)avgFPS);
                SDL_SetWindowTitle(window, windowTitle.c_str());
            }

            countedFrames++;
        }
    }

    close(&window, &renderer);

    return 0;
}
