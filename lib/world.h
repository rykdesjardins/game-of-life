#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "organism.h"
#include <cstdlib>
#include <iostream>
using namespace std;

namespace GameOfLife 
{
    const int WORLD_SIZE = 1000;
    const int FAMILY_COUNT = 20;
    const int FIELDS_COUNT = 60;

    const int FAMILY_MIN_SIZE = 5;
    const int FAMILY_MAX_SIZE = 12;

    const int FIELD_MIN_SIZE = 20;
    const int FIELD_MAX_SIZE = 100;

    struct Tile
    {
        Organism *organism;
        Plant *plant;
        int x;
        int y;
    };

    class World
    {
        Tile **tiles;
        vector<Organism*> organisms;
        vector<Plant*> plants;

        public:
            World();
            
    };
}

#endif
