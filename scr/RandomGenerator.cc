#include "RandomGenerator.h"

RandomGenerator::RandomGenerator( int s){
    seed = s;
    std::srand(seed);
}
int RandomGenerator::rand_int(int lo, int hi){
    int n = hi - lo;
    return lo + std::rand() % n;

}
void RandomGenerator::setSeed(int s){seed = s; std::srand(seed);}