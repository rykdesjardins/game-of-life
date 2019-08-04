#include "../lib/organism.h"

namespace GameOfLife 
{
    Organism::Organism(int x, int y, int id) 
    {
        this->x = x;
        this->y = y;
        this->familyId = id;

        this->gender = (Gender)(rand() % GENDER_TOTAL);
        this->personality = (Personality)(rand() % PERSONALITY_TOTAL);
    }

    Plant::Plant(int x, int y, int id)
    {
        this->x = x;
        this->y = y;
        this->id = id;
    }

    void Organism::Tick()
    {
        this->energy--;
        this->hydratation--;
        this->age++;

        if (this->awake)
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

        if (this->age == this->lifespan || this->energy == 0)
        {
            this->alive = false;
        }
    }
}
