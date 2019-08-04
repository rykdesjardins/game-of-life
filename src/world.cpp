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
                cout << "Family " << i << ", organism " << j << " position " << root_x << ", " << root_y << endl;
                if (rand() % 2 == 0)
                    root_x++;
                else
                    root_y++;

                if (root_x >= WORLD_SIZE || root_y >= WORLD_SIZE) break;

                if (this->tiles[root_x][root_y].organism == nullptr) 
                {
                    Organism org(root_x, root_y);
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
                cout << "Field " << i << ", plant " << j << " position " << root_x << ", " << root_y << endl;

                if (rand() % 2 == 0)
                    root_x++;
                else
                    root_y++;

                if (root_x >= WORLD_SIZE || root_y >= WORLD_SIZE) break;

                if (this->tiles[root_x][root_y].plant == nullptr) 
                {
                    Plant pl(root_x, root_y, j);
                    this->plants.push_back(&pl);
                    this->tiles[root_x][root_y].plant = &pl;
                }
            }
        }

        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int y = 0; y < WORLD_SIZE; y++) {
                if (this->tiles[x][y].organism != nullptr) cout << "O";
                else if (this->tiles[x][y].plant != nullptr) cout << "Y";
                else cout << " ";
            }

            cout << endl;
        }
    }
}
