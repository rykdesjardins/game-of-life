#ifndef WORLD_H
#define WORLD_H

#ifndef MEM_MULTIPLIER
    #define MEM_MULTIPLIER 1
#endif

#include <vector>
#include <algorithm>
#include "organism.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

namespace GameOfLife 
{
    const int WORLD_SIZE = 1000 * MEM_MULTIPLIER;
    const int FAMILY_COUNT = 20 * MEM_MULTIPLIER;
    const int FIELDS_COUNT = 20 * MEM_MULTIPLIER;
    const int WATER_PATCH_COUNT = 200 * MEM_MULTIPLIER;
    const int WATER_PATCH_SIZE = 100 * MEM_MULTIPLIER;

    const int FAMILY_MIN_SIZE = 10 * MEM_MULTIPLIER;
    const int FAMILY_MAX_SIZE = 20 * MEM_MULTIPLIER;

    const int FIELD_MIN_SIZE = 2 * MEM_MULTIPLIER;
    const int FIELD_MAX_SIZE = 10 * MEM_MULTIPLIER;

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
        Tile **tiles;
        vector<Organism*> organisms;
        vector<Plant*> plants;

        public:
            World();
            bool Tick();
            void GetVisible(vector<TileDistance>&, Tile*, int);
    };
}

#endif
