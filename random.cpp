#include "random.h"
#include <chrono>

Random::Random() : mersenneTwister(randDevice())
{
    mersenneTwister.seed((std::chrono::system_clock::now().time_since_epoch()).count());
}

int Random::randomInt(int minVal, int maxVal)
{
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    return dist(mersenneTwister);
}

double Random::randomDouble(double minVal, double maxVal)
{
    std::uniform_real_distribution<double> dist(minVal, maxVal);
    return dist(mersenneTwister);
}

bool Random::randomTrue(double pct)
{
    if (pct <= 0.0) return false;
    if (pct >= 1.0) return true;
    std::uniform_real_distribution<double> dist(0, 1);
    return dist(mersenneTwister) <= pct;
}
