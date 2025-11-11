/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<std::floating_point T> inline constexpr T Rad2Deg = static_cast<T>(57.2957795130823);
    inline constexpr double Rad2DegD = Rad2Deg<double>;
    inline constexpr float Rad2DegF = Rad2Deg<float>;

    template<std::floating_point T> inline constexpr T Deg2Rad = static_cast<T>(0.0174532925199433);
    inline constexpr double Deg2RadD = Deg2Rad<double>;
    inline constexpr float Deg2RadF = Deg2Rad<float>;

    template<std::floating_point T> inline constexpr T Epsilon = static_cast<T>(1e-4);
    inline constexpr double EpsilonD = Epsilon<double>;
    inline constexpr float EpsilonF = Epsilon<float>;
}

namespace TG
{
    constexpr long double operator""_deg_to_rad(long double degree)
    {
        const long double radian = degree * std::numbers::pi_v<long double> / 180;
        return radian;
    }

    constexpr long double operator""_rad_to_deg(long double radian)
    {
        const long double degree = radian * 180 / std::numbers::pi_v<long double>;
        return degree;
    }
}
