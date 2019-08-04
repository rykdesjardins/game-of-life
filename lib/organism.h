#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
#include <vector>
using namespace std;

namespace GameOfLife 
{
    // Forward declaration
    class World;
    struct TileDistance;
    struct Tile;
    
    const int GENDER_TOTAL = 5;
    enum Gender
    {
        SIGMA, OMICRON, THETA, PHI, ZETA
    };

    enum State 
    {
        STALE,      // Not doing anything
        EATING,     // Injesting food 
        REPRO,      // Reproduction
        FIGHTING,   // Fighting for food
        SHARING     // Sharing information (not implemented)
    };

    const int PERSONALITY_TOTAL = 4;
    enum Personality
    {
        EGOCENTRIC, // Food and survival first
        ECOLOGIC,   // Always eat at the center of crops
        LAZY,       // Nearest point of interest
        ALPHA       // Reproduction first
    };

    const int DRAWN_DIRECTION_TOTAL = 4;
    enum DrawnDirection 
    {
        NORTHWEST, NORTHEAST, SOUTHEAST, SOUTHWEST
    };

    enum OrganismPriority 
    {
        EXPLORATION, FOOD, WATER, REPRODUCTION
    };

    class Plant
    {
        Tile* tile;
        int id;
        int age = 0;
        int energy = 20;
        int energy_needed = 5;
        int time_to_renew = 100;

        public:
            int GetID() { return this->id; };
            int GetTotalEnergy() { return this->energy - this->energy_needed; }
            int GetEnergyNeeded() { return this->energy_needed; }
            Tile* GetTile() { return this->tile; };
            void Tick();
            bool CanRenew() { return this->age % this->time_to_renew == 0; };
            Plant(int, Tile*);
            ~Plant();
    };

    class Organism 
    {
        Gender gender = SIGMA;
        State state = STALE;
        Personality personality = EGOCENTRIC;
        DrawnDirection drawndirection;

        static int death_by_dehydration;
        static int death_by_starvation;
        static int death_by_aging;
        static int total_organisms;
        static int total_offsprings;
        static int total_alive;
        static int total_generations;

        int parent1id;
        int parent2id; 
        int generation;
        int offsprings;

        bool alive = true;
        bool awake = true;
        int sight = 4;
        int velocity = 4;

        int lifespan;
        int age = 0;
        int energy;
        int max_energy;
        int hydratation;
        int max_hydratation;
        int strength = 100;

        float min_temperature = -30;
        float max_temperature = 30;

        int max_awake_time = 16;
        int max_sleep_time = 8;
        int awake_time = 0;
        int sleep_time = 0;
        Tile* currenttile;

        public: 
            Organism(int, Tile*);
            Organism(Organism*, Organism*, Tile*);
            ~Organism();

            Tile* GetTile() { return this->currenttile; };

            bool IsHungry();
            bool IsThirsty();
            bool CanBreed();
            bool CanBreedWith(Organism*);
            bool Tick();
            Tile* Move(World*);
            void Feed(Plant*);
            void Drink();

            static void PrintStats();
    };
}

#endif
