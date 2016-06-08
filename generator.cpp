#include "generator.h"

unsigned int generator_without_limitation()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 r (seed);
    return r();
}

double generator_null_one() //генерирует число от 0 до 1
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::uniform_real_distribution<double> distribution (0.0,1.0);
    return distribution(generator);
}
