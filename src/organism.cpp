#include "../lib/organism.h"
#include "../lib/world.h"

namespace GameOfLife 
{
    int Organism::death_by_aging = 0;
    int Organism::death_by_dehydration = 0;
    int Organism::death_by_starvation = 0;
    int Organism::total_organisms = 0;
    int Organism::total_offsprings = 0;

    Organism::Organism(int id, Tile* tile) 
    {
        this->familyId = id;
        this->currenttile = tile;

        this->gender = (Gender)(rand() % GENDER_TOTAL);
        this->personality = (Personality)(rand() % PERSONALITY_TOTAL);
        this->drawndirection = (DrawnDirection)(rand() % DRAWN_DIRECTION_TOTAL);

        this->lifespan = (rand() % (200 * MEM_MULTIPLIER)) + (50 * MEM_MULTIPLIER);
        this->max_energy = (rand() % (100 * MEM_MULTIPLIER)) + (10 * MEM_MULTIPLIER);
        this->max_hydratation = (rand() % (20 * MEM_MULTIPLIER)) + (10 * MEM_MULTIPLIER);

        this->min_temperature = (float)500 / (rand() % 10);
        this->max_temperature = (float)500 / (rand() % 10);

        this->energy = this->max_energy;
        this->hydratation = this->max_hydratation;

        tile->organism = this;
        Organism::total_organisms++;
    }

    Organism::~Organism()
    {
        this->currenttile->organism = nullptr;
    }

    void Organism::PrintStats()
    {
        cout << "Total organisms " << Organism::total_organisms << endl;
        cout << "Dead by aging " << Organism::death_by_aging << endl;
        cout << "Dead by starvation " << Organism::death_by_starvation << endl;
        cout << "Dead by dehydration " << Organism::death_by_dehydration << endl;
    }

    bool Organism::Tick()
    {
        this->energy--;
        this->hydratation--;
        this->age++;

        if (this->age == this->lifespan) 
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
        return this->CanBreed() && this->familyId == right->familyId;
    }

    bool Organism::CanBreed()
    {
        return this->age > 1000;
    }

    Tile* Organism::Move(World* world)
    {
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
                            if (td.tile->organism != nullptr && !td.tile->organism->CanBreedWith(this)) { 
                                candidate = &td; 
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
                if (td.distance > candidate->distance && td.tile->organism == nullptr)
                {
                    candidate = &td;
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
    }

    Plant::~Plant()
    {
        this->tile->plant = nullptr;
    }

    void Plant::Tick()
    {
        this->age++;
    }

    bool Plant::CanRenew() 
    {
        return this->age % this->time_to_renew == 0;
    }
}
