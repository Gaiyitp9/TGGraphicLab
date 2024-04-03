/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <concepts>

namespace TG::Math
{
    template <class T>
    struct Invalid
    {
        static_assert(false, "A program that instantiates a primary template of a mathematical constant "
                             "variable template is ill-formed.");
    };

    template<typename T> inline constexpr T Rad2Deg_v = Invalid<T>{};
    template<typename T> inline constexpr T Deg2Rad_v = Invalid<T>{};

    template<std::floating_point T> inline constexpr T Rad2Deg_v<T> = static_cast<T>(57.2957795130823);
    template<std::floating_point T> inline constexpr T Deg2Rad_v<T> = static_cast<T>(0.0174532925199433);

    inline constexpr double Rad2Deg = Rad2Deg_v<double>;
    inline constexpr double Deg2Rad = Rad2Deg_v<double>;

	// 地址是否满足对齐条件
	template<typename T>
	inline bool IsAligned(T const* value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}

	template<typename T> inline T Abs(T a) { return a < 0 ? -a : a; }
}
