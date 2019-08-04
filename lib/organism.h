#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
using namespace std;

namespace GameOfLife 
{
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

    class Organism 
    {
        Gender gender = SIGMA;
        State state = STALE;
        Personality personality = EGOCENTRIC;

        int familyId;
        int x;
        int y;

        bool alive = true;
        bool awake = true;
        int sight = 4;
        int lifespan = 100000;
        int energy = 1000;
        int max_energy = 1000;
        int age = 0;
        int velocity = 4;
        int strength = 100;
        int hydratation = 1000;
        int max_hydratation = 1000;
        float min_temperature = -30;
        float max_temperature = 30;
        int max_awake_time = 16;
        int max_sleep_time = 8;
        int awake_time = 0;
        int sleep_time = 0;

        public: 
            Organism(int, int, int);
            Organism(Organism*, Organism*);

            void Tick();
    };

    class Plant
    {
        int id;
        int age;
        int energy;
        int energy_needed;
        int time_to_renew;
        int x;
        int y;

        public:
            Plant(int, int, int);
    };
}

#endif
