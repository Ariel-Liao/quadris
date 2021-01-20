#ifndef RANDOM_H
#define RANDOM_H
#include <cstdlib>

class RandomGenerator{
  int seed;

  public:
    RandomGenerator(int seed = 0);
    int rand_int(int lo, int hi);
    void setSeed(int s);
};
#endif
