#include "../lib/organism.h"
#include "../lib/world.h"

namespace GameOfLife 
{
    int Organism::death_by_aging = 0;
    int Organism::death_by_dehydration = 0;
    int Organism::death_by_starvation = 0;
    int Organism::total_organisms = 0;
    int Organism::total_offsprings = 0;
    int Organism::total_alive = 0;
    int Organism::total_generations = 0;

    Organism::Organism(int id, Tile* tile) 
    {
        this->parent1id = id;
        this->parent2id = id;
        this->currenttile = tile;
        this->generation = 1;

        this->gender = (Gender)(rand() % GENDER_TOTAL);
        this->personality = (Personality)(rand() % PERSONALITY_TOTAL);
        this->drawndirection = (DrawnDirection)(rand() % DRAWN_DIRECTION_TOTAL);

        this->lifespan = (rand() % (100 * MEM_MULTIPLIER)) + (20 * MEM_MULTIPLIER);
        this->max_energy = (rand() % (100 * MEM_MULTIPLIER)) + (10 * MEM_MULTIPLIER);
        this->max_hydratation = (rand() % (20 * MEM_MULTIPLIER)) + (10 * MEM_MULTIPLIER);

        this->min_temperature = (float)-500 / (rand() % 10);
        this->max_temperature = (float)500 / (rand() % 10);

        this->energy = this->max_energy;
        this->hydratation = this->max_hydratation;

        int daytime = rand() % 10 + 10;
        this->max_awake_time = daytime;
        this->max_sleep_time = 24 - daytime;

        tile->organism = this;
        Organism::total_organisms++;
        Organism::total_alive++;
        Organism::total_generations = 1;
    }

    Organism::Organism(Organism* parent1, Organism* parent2, Tile* tile)
    {
        this->parent1id = parent1->parent1id;
        this->parent2id = parent2->parent2id;
        this->currenttile = tile;
        this->generation = max(parent1->generation, parent2->generation) + 1;

        this->gender = (Gender)(rand() % GENDER_TOTAL);
        this->personality = (Personality)(rand() % PERSONALITY_TOTAL);
        this->drawndirection = (DrawnDirection)(rand() % DRAWN_DIRECTION_TOTAL);

        this->lifespan = max(parent1->lifespan, parent2->lifespan) * 1.01;
        this->max_energy = max(parent1->max_energy, parent2->max_energy) * 1.1;
        this->max_hydratation = max(parent1->max_hydratation, parent2->max_hydratation) * 1.1;

        this->min_temperature = min(parent1->min_temperature, parent2->min_temperature) * 1.1;
        this->max_temperature = max(parent1->max_temperature, parent2->max_temperature) * 1.1;

        this->energy = this->max_energy;
        this->hydratation = this->max_hydratation;
        
        int daytime = (parent1->max_awake_time + parent2->max_awake_time) / 2;
        this->max_awake_time = daytime;
        this->max_sleep_time = 24 - daytime;

        tile->organism = this;
        Organism::total_organisms++;
        Organism::total_offsprings++;
        Organism::total_alive++;
        Organism::total_generations = max(Organism::total_generations, this->generation);

        parent1->offsprings++;
        parent2->offsprings++;
    }

    Organism::~Organism()
    {
        this->currenttile->organism = nullptr;
        Organism::total_alive--;
    }

    void Organism::PrintStats()
    {
        cout << "Total organisms     " << Organism::total_organisms << endl;
        cout << "Total alive         " << Organism::total_alive << endl;
        cout << "Total generations   " << Organism::total_generations << endl;
        cout << "Dead by aging       " << Organism::death_by_aging << endl;
        cout << "Dead by starvation  " << Organism::death_by_starvation << endl;
        cout << "Dead by dehydration " << Organism::death_by_dehydration << endl;
    }

    bool Organism::Tick()
    {
        this->energy--;
        this->hydratation--;
        this->age++;

        if (this->age >= this->lifespan) 
        {
            Organism::death_by_aging++;
            this->alive = false;
        } 
        else if (this->energy == 0)
        {
            Organism::death_by_starvation++;
            this->alive = false;
        }
        else if (this->hydratation == 0)
        {
            Organism::death_by_dehydration++;
            this->alive = false;
        }
        else if (this->awake)
        {
            this->awake_time++;

            if (this->awake_time == this->max_awake_time) 
            {
                this->awake_time = 0;
                this->awake = false;
            }
        }
        else
        {
            this->sleep_time++;
            
            if (this->sleep_time == this->max_sleep_time) 
            {
                this->sleep_time = 0;
                this->awake = true;
            }
        }

        return this->alive;
    }

    bool Organism::IsHungry()
    {
        float ratio = this->energy / this->max_energy;
        return ratio < 0.50;
    }

    bool Organism::IsThirsty()
    {
        float ratio = this->hydratation / this->max_hydratation;
        return ratio < 0.50;
    }

    bool Organism::CanBreedWith(Organism *right)
    {
        return this->CanBreed() && this->parent1id != right->parent1id && this->parent2id != right->parent2id && this->gender != right->gender;;
    }

    bool Organism::CanBreed()
    {
        return this->offsprings < (MEM_MULTIPLIER) && this->age > 60 * MEM_MULTIPLIER;
    }

    Tile* Organism::Move(World* world)
    {
        if (!this->awake) { return nullptr; }

        OrganismPriority priority = EXPLORATION;
        vector<TileDistance> visibleTiles;
        world->GetVisible(visibleTiles, this->currenttile, this->sight);
        TileDistance* candidate = nullptr; 

        if (this->IsThirsty()) priority = WATER;
        else if (this->IsHungry()) priority = FOOD;
        else if (this->CanBreed()) priority = REPRODUCTION;

        if (priority != EXPLORATION) 
        {
            for (TileDistance &td : visibleTiles)
            {
                if (candidate == nullptr || td.distance == candidate->distance)
                {
                    switch (priority) 
                    {
                        case FOOD:
                            if (td.tile->plant != nullptr && td.tile->organism == nullptr) { candidate = &td; }
                            break;

                        case WATER:
                            if (td.tile->isWater && td.tile->organism == nullptr) { candidate = &td; }
                            break;

                        case REPRODUCTION:
                            if (td.tile->organism != nullptr && this->CanBreedWith(td.tile->organism)) { 
                                vector<TileDistance> emptyTilesAroundMate;
                                world->GetVisible(emptyTilesAroundMate, td.tile, 1);
                                for (TileDistance &td : emptyTilesAroundMate) 
                                {
                                    if (td.tile->organism == nullptr) 
                                    {
                                        candidate = &td; 
                                        break;
                                    }
                                }
                            }
                            break;

                        default: break;
                    }
                } 
            }
        } 
       
        if (candidate == nullptr) {
            candidate = &visibleTiles[0];
            for (TileDistance &td : visibleTiles)
            {
                if (td.distance > candidate->distance && td.tile->organism == nullptr) {
                    switch (this->drawndirection) {
                        case NORTHWEST:
                            if (candidate->tile->x < this->currenttile->x && candidate->tile->y < this->currenttile->y)
                                candidate = &td;
                            break;

                        case NORTHEAST:
                            if (candidate->tile->x > this->currenttile->x && candidate->tile->y < this->currenttile->y)
                                candidate = &td;
                            break;

                        case SOUTHWEST:
                            if (candidate->tile->x > this->currenttile->x && candidate->tile->y > this->currenttile->y)
                                candidate = &td;
                            break;

                        case SOUTHEAST:
                            if (candidate->tile->x < this->currenttile->x && candidate->tile->y < this->currenttile->y)
                                candidate = &td;
                            break;
                    }
                }
            }
        }

        if (candidate != nullptr)
        {
            this->currenttile->organism = nullptr;
            this->currenttile = candidate->tile;
            candidate->tile->organism = this;
            return this->currenttile;
        } 
        else 
        {
            return nullptr;
        }
    }

    void Organism::Feed(Plant* plant)
    {
        this->energy += plant->GetTotalEnergy();
        if (this->energy > this->max_energy) this->energy = this->max_energy;
    }

    void Organism::Drink()
    {
        this->hydratation = this->max_hydratation;
    }

    Plant::Plant(int id, Tile* tile)
    {
        this->id = id;
        this->tile = tile;
        tile->plant = this;

        this->energy = rand() % 20 * MEM_MULTIPLIER;
        this->energy_needed = rand() % 2 * MEM_MULTIPLIER;
        this->time_to_renew = (rand() % 100 + 20) * MEM_MULTIPLIER;
    }

    Plant::~Plant()
    {
        this->tile->plant = nullptr;
    }

    void Plant::Tick()
    {
        this->age++;
    }
}
