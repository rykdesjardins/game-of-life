#include "lib/world.h"
using namespace GameOfLife;

int main() {
    World* world = new World();
    while(world->Tick());
    Organism::PrintStats();

    delete world;
    return 0;
}
