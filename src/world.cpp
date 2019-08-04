#include "../lib/world.h"

namespace GameOfLife
{
    World::World() 
    {
        srand( time(NULL) );
        this->tiles = new Tile*[WORLD_SIZE];
        for (int i = 0; i < WORLD_SIZE; i++)
        {
            this->tiles[i] = new Tile[WORLD_SIZE];
        }

        // Families
        for (int i = 0; i < FAMILY_COUNT; i++)
        {
            // Avoiding edge of the map
            int root_x = abs(rand() % (WORLD_SIZE + FAMILY_MAX_SIZE) - FAMILY_MAX_SIZE);
            int root_y = abs(rand() % (WORLD_SIZE + FAMILY_MAX_SIZE) - FAMILY_MAX_SIZE);
            int family_size = rand() % (FAMILY_MAX_SIZE + FAMILY_MIN_SIZE) - FAMILY_MIN_SIZE + 1;

            for (int j = 0; j < family_size; j++) 
            {
                if (rand() % 2 == 0) root_x++;
                else root_x--;
                if (rand() % 2 == 0) root_y++;
                else root_y--;

                if (root_x >= WORLD_SIZE || root_y >= WORLD_SIZE || root_x < 0 || root_y < 0) continue;

                if (this->tiles[root_x][root_y].organism == nullptr) 
                {
                    Organism org(root_x, root_y, i);
                    this->organisms.push_back(&org);
                    this->tiles[root_x][root_y].organism = &org;
                }
            }
        }

        // Plants
        for (int i = 0; i < FIELDS_COUNT; i++)
        {
            int root_x = abs(rand() % (WORLD_SIZE + FIELD_MAX_SIZE) - FIELD_MAX_SIZE);
            int root_y = abs(rand() % (WORLD_SIZE + FIELD_MAX_SIZE) - FIELD_MAX_SIZE);
            int field_size = rand() % (FIELD_MAX_SIZE + FIELD_MIN_SIZE) - FIELD_MIN_SIZE + 1;

            for (int j = 0; j < field_size; j++) 
            {
                if (rand() % 2 == 0) root_x++;
                else root_x--;
                if (rand() % 2 == 0) root_y++;
                else root_y--;

                if (root_x >= WORLD_SIZE || root_y >= WORLD_SIZE || root_x < 0 || root_y < 0) continue;

                if (this->tiles[root_x][root_y].plant == nullptr) 
                {
                    Plant pl(root_x, root_y, j);
                    this->plants.push_back(&pl);
                    this->tiles[root_x][root_y].plant = &pl;
                }
            }
        }

        // Water
        for (int i = 0; i < WATER_PATCH_COUNT; i++)
        {
            int root_x = abs(rand() % (WORLD_SIZE + FIELD_MAX_SIZE) - FIELD_MAX_SIZE);
            int root_y = abs(rand() % (WORLD_SIZE + FIELD_MAX_SIZE) - FIELD_MAX_SIZE);

            for (int j = 0; j < WATER_PATCH_SIZE; j++)
            {
                if (rand() % 2 == 0) root_x++;
                else root_x--;
                if (rand() % 2 == 0) root_y++;
                else root_y--;

                if (root_x >= WORLD_SIZE || root_y >= WORLD_SIZE || root_x < 0 || root_y < 0) continue;

                this->tiles[root_x][root_y].isWater = true;
            }
        }
    }

    void World::tick()
    {
        for (Organism *org : this->organisms)
        {
            org->Tick();
        }
    }
}
