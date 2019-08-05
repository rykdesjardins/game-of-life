#ifndef WORLD_H
#define WORLD_H

#ifndef MEM_MULTIPLIER
    #define MEM_MULTIPLIER 4
#endif

#include <vector>
#include <algorithm>
#include "organism.h"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

namespace GameOfLife 
{
    const int GAME_BASE_SIZE = 700;
    const int WORLD_SIZE = GAME_BASE_SIZE * MEM_MULTIPLIER;
    const int FAMILY_COUNT = 40 * MEM_MULTIPLIER;
    const int FIELDS_COUNT = 100 * MEM_MULTIPLIER;
    const int WATER_PATCH_COUNT = 100 * MEM_MULTIPLIER;
    const int WATER_PATCH_SIZE = 1000 * MEM_MULTIPLIER;

    const int FAMILY_MIN_SIZE = 10 * MEM_MULTIPLIER;
    const int FAMILY_MAX_SIZE = 50 * MEM_MULTIPLIER;

    const int FIELD_MIN_SIZE = 50 * MEM_MULTIPLIER;
    const int FIELD_MAX_SIZE = 150 * MEM_MULTIPLIER;

    struct Tile
    {
        Organism *organism;
        Plant *plant;
        int x;
        int y;
        bool isWater = false;
    };

    struct TileDistance
    {
        Tile* tile;
        int distance;

        TileDistance(const TileDistance&);
        TileDistance(Tile*, int);
        TileDistance();
    };

    class World
    {
        int age;
        Tile **tiles;
        vector<Organism*> organisms;
        vector<Plant*> plants;

        public:
            World();
            bool Tick();
            void GetVisible(vector<TileDistance>&, Tile*, int);
            Tile** getTiles() { return this->tiles; };
    };
}

#endif
