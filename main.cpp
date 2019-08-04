#include "lib/world.h"

using namespace GameOfLife;

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  cout << "Crashed" << endl;
  sleep(1);
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main() {
    signal(SIGSEGV, handler);
    World* world = new World();
    while(world->Tick());
    Organism::PrintStats();

    delete world;
    return 0;
}
