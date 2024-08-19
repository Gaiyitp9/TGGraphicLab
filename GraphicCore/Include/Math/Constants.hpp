/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <concepts>
#include <numbers>

namespace TG::Math
{
    template<std::floating_point T> inline constexpr T Rad2DegV = static_cast<T>(57.2957795130823);
    template<std::floating_point T> inline constexpr T Deg2RadV = static_cast<T>(0.0174532925199433);

    inline constexpr double Rad2Degd = Rad2DegV<double>;
    inline constexpr double Deg2Radd = Deg2RadV<double>;
    inline constexpr double Rad2Degf = Rad2DegV<float>;
    inline constexpr double Deg2Radf = Deg2RadV<float>;

    constexpr long double operator""_deg_to_rad(long double degree)
    {
        long double radian = degree * std::numbers::pi_v<long double> / 180;
        return radian;
    }

    constexpr long double operator""_rad_to_deg(long double radian)
    {
        long double degree = radian * 180 / std::numbers::pi_v<long double>;
        return degree;
    }
}
