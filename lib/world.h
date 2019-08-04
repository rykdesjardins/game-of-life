#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "organism.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#define MEM_MULTIPLIER 10;

namespace GameOfLife 
{
    const int WORLD_SIZE = 1000 * MEM_MULTIPLIER;
    const int FAMILY_COUNT = 2 * MEM_MULTIPLIER;
    const int FIELDS_COUNT = 6 * MEM_MULTIPLIER;
    const int WATER_PATCH_COUNT = 10 * MEM_MULTIPLIER;
    const int WATER_PATCH_SIZE = 10 * MEM_MULTIPLIER;

    const int FAMILY_MIN_SIZE = 5 * MEM_MULTIPLIER;
    const int FAMILY_MAX_SIZE = 10 * MEM_MULTIPLIER;

    const int FIELD_MIN_SIZE = 2 * MEM_MULTIPLIER;
    const int FIELD_MAX_SIZE = 10 * MEM_MULTIPLIER;

    struct Tile
    {
        Organism *organism;
        Plant *plant;
        bool isWater = false;
    };

    class World
    {
        Tile **tiles;
        vector<Organism*> organisms;
        vector<Plant*> plants;

        public:
            World();
            void tick();
            
    };
}

#endif
