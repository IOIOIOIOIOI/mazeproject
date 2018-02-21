#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <algorithm>

class Random {
    std::random_device randDevice;
    std::mt19937 mersenneTwister;
public:
    Random();
    int randomInt(int minVal, int maxVal);
    double randomDouble(double minVal, double maxVal);
    bool randomTrue(double pct);
    template<typename T> void shuffle(T first, T last);
    template<typename T> void shuffle(T& container);
    template<typename T> T shuffled(T container);
};

template<typename T>
void Random::shuffle(T first, T last)
{
    std::shuffle(first, last, mersenneTwister);
}

template<typename T>
void Random::shuffle(T& container)
{
    std::shuffle(std::begin(container), std::end(container), mersenneTwister);
}

template<typename T>
T Random::shuffled(T container)
{
    Random::shuffle(container);
    return std::move(container);
}

#endif // RANDOM_H
