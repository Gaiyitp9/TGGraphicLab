/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <concepts>
#include <numbers>

namespace TG::Math
{
    template <class T>
    struct Invalid
    {
        static_assert(false, "A program that instantiates a primary template of a mathematical constant "
                             "variable template is ill-formed.");
    };

    template<typename T> inline constexpr T Rad2DegV = Invalid<T>{};
    template<typename T> inline constexpr T Deg2RadV = Invalid<T>{};

    template<std::floating_point T> inline constexpr T Rad2DegV<T> = static_cast<T>(57.2957795130823);
    template<std::floating_point T> inline constexpr T Deg2RadV<T> = static_cast<T>(0.0174532925199433);

    inline constexpr double Rad2Deg = Rad2DegV<double>;
    inline constexpr double Deg2Rad = Deg2RadV<double>;

//    inline constexpr double operator""_deg_to_rad(long double degree)
//    {
//        double radians = degree * std::numbers::pi_v<long double> / 180;
//        return radians;
//    }
}
