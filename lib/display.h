#include <SDL2/SDL.h>
#include "world.h"

namespace GameOfLife
{
    class Display
    {
        int whandle;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture;
        SDL_Event event;
        float w = GAME_BASE_SIZE;
        float h = GAME_BASE_SIZE;

        World* world;
        void Print();

        public:
            Display(World*);
            ~Display();
            void Show();
            bool Update();
    };
}
