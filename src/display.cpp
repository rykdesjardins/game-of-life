#include "../lib/display.h"

namespace GameOfLife
{
    Display::Display(World* world)
    {
        SDL_Init(SDL_INIT_VIDEO);
        this->world = world;
    }

    Display::~Display()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Display::Show()
    {
        SDL_CreateWindowAndRenderer(GAME_BASE_SIZE, GAME_BASE_SIZE, SDL_WINDOW_RESIZABLE, &window, &renderer);
    }

    void Display::Print()
    {
        float base = (float)GAME_BASE_SIZE;
        int ratio_x = (base / w) * MEM_MULTIPLIER;
        int ratio_y = (base / h) * MEM_MULTIPLIER;
        Tile** tiles = this->world->getTiles();

        for (int x = 0; x < WORLD_SIZE; x++) 
        {
            for (int y = 0; y < WORLD_SIZE; y++)
            {
                Tile* tile = &tiles[x][y];

                if (tile->isWater) 
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 / MEM_MULTIPLIER);
                    SDL_RenderDrawPoint(renderer, x / ratio_x, y / ratio_y);
                }
                if (tile->plant != nullptr) 
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255 / MEM_MULTIPLIER);
                    SDL_RenderDrawPoint(renderer, x / ratio_x, y / ratio_y);
                }
                if (tile->organism != nullptr) 
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 / MEM_MULTIPLIER);
                    SDL_RenderDrawPoint(renderer, x / ratio_x, y / ratio_y);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    bool Display::Update()
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) { return false; } 
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) 
        {
            this->w = (float)event.window.data1;
            this->h = (float)event.window.data2;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        this->Print();

        return true;
    }
}
