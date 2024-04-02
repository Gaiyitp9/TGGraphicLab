#pragma once

#include <numbers>
#include <random>

inline constexpr double Epsilon = 1e-5;

inline double Deg2Rad(double degree)
{
    return degree * std::numbers::pi / 180.0;
}

inline double RandomDouble()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

inline double Linear2Gamma(double linearComponent)
{
    return std::sqrt(linearComponent);
}
