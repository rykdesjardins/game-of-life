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

            for (int j = 0; j < WORLD_SIZE; j++)
            {
                this->tiles[i][j].x = i;
                this->tiles[i][j].y = j;
            }
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
                    Organism* org = new Organism(i, &this->tiles[root_x][root_y]);
                    this->organisms.push_back(org);
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
                    Plant* pl = new Plant(j, &this->tiles[root_x][root_y]);
                    this->plants.push_back(pl);
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

    bool World::Tick()
    {
        this->age++;
        vector<Organism*> newOrgs;
        vector<Plant*> newPlants;
        vector<Organism*>::iterator oit = this->organisms.begin();

        while (oit != this->organisms.end())
        {
            Organism * org = (*oit);
            if (org->Tick()) 
            {
                Tile* newtile = org->Move(this);
                if (newtile != nullptr)
                {
                    if (newtile->plant != nullptr && org->IsHungry())
                    {
                        Plant *plant = newtile->plant;
                        org->Feed(plant);
                        this->plants.erase(find(this->plants.begin(), this->plants.end(), plant));
                        delete plant;
                    }

                    if (newtile->isWater && org->IsThirsty())
                    {
                        org->Drink();
                    }

                    if (org->CanBreed())
                    {
                        vector<TileDistance> around;
                        this->GetVisible(around, org->GetTile(), 1);

                        for (TileDistance &dt : around)
                        {
                            if (dt.tile->organism != nullptr && org->CanBreedWith(dt.tile->organism))
                            {
                                Organism * mate = dt.tile->organism;
                                for (TileDistance &emptydt : around) 
                                {
                                    if (emptydt.tile->organism == nullptr) 
                                    {
                                        Organism * offspring = new Organism(org, mate, emptydt.tile);
                                        newOrgs.push_back(offspring);
                                        break;
                                    }
                                }

                                break;
                            }
                        }
                    }
                }

                oit++;
            } 
            else 
            {
                this->organisms.erase(oit);
                delete org;
            }
        }

        for (Organism *org : newOrgs)
        {
            this->organisms.push_back(org);
        }
        newOrgs.empty();

        for (Plant *plant : this->plants)
        {
            plant->Tick();
            if (plant->CanRenew())
            {
                vector<TileDistance> surrounds;
                this->GetVisible(surrounds, plant->GetTile(), 1);

                for (TileDistance &dt : surrounds)
                {
                    if (dt.tile->plant == nullptr) 
                    {
                        Plant* pl = new Plant(plant->GetID(), dt.tile);
                        newPlants.push_back(pl);

                        break;
                    }
                }
            }
        }

        for (Plant *pl : newPlants)
        {
            this->plants.push_back(pl);
        }
        newPlants.empty();

        cout << "WORLD HOUR " << this->age << endl;
        cout << "Total plants        " << this->plants.size() << endl;
        Organism::PrintStats();

        usleep(1);
        return this->organisms.size() != 0;
    }

    void World::GetVisible(vector<TileDistance> &tiles, Tile* tile, int radius)
    {
        int ox = tile->x;
        int oy = tile->y;
        for (int x = ox - radius; x < ox + radius; x++)
        {
            for (int y = oy - radius; y < oy + radius; y++)
            {
                if (x >= 0 && x < WORLD_SIZE && y >= 0 && y < WORLD_SIZE)
                {
                    TileDistance td(&this->tiles[x][y], abs(x - ox) + abs(y - oy));
                    tiles.push_back(td);
                }
            }
        }
    }

    TileDistance::TileDistance() {}
    TileDistance::TileDistance(const TileDistance& o)
    {
        this->tile = o.tile;
        this->distance = o.distance;
    }

    TileDistance::TileDistance(Tile *tile, int distance)
    {
        this->tile = tile;
        this->distance = distance;
    }
}
