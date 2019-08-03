#include "../lib/organism.h"

namespace GameOfLife 
{
    Organism::Organism(int x, int y) 
    {
        this->x = x;
        this->y = y;
    }

    Plant::Plant(int x, int y, int id)
    {
        this->x = x;
        this->y = y;
        this->id = id;
    }
}
