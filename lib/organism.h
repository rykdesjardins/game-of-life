#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
using namespace std;

namespace GameOfLife 
{
    enum Gender
    {
        SIGMA, OMICRON, THETA, PHI, ZETA
    };

    enum State 
    {
        STALE, EATING, REPRO, FIGHTING, SHARING
    };

    enum Personality
    {
        EGOCENTRIC, // Food and survival first
        ECOLOGIC,   // Always eat at the center of crops
        LAZY,       // Nearest point of interest
        ALPHA       // Reproduction first
    };

    class Organism 
    {
        Gender gender;
        State state;
        Personality personality;

        int sight;
        int lifespan;
        int energy;
        int max_energy;
        int age;
        int familyId;
        int velocity;
        int strength;
        int x;
        int y;
        float min_temperature;
        float max_temperature;
        int awake_time;

        public: 
            Organism(int, int);
            Organism(Organism, Organism);

            void Play();
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
