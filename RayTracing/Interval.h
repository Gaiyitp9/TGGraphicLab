#pragma once

#include <limits>

class Interval
{
public:
    Interval() : min(std::numeric_limits<double>::infinity()), max(std::numeric_limits<double>::infinity()) {}
    Interval(double min, double max) : min(min), max(max) {}

    [[nodiscard]] bool Contains(double x) const
    {
        return min <= x && x <= max;
    }

    [[nodiscard]] bool Surrounds(double x) const
    {
        return min < x && x < max;
    }

    [[nodiscard]] double Clamp(double x) const
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    double min, max;
};
