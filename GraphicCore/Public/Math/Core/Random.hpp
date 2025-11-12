/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <random>

namespace TG::Math
{
    class Random
    {
    public:
        Random() : m_generator(std::random_device{}()) {}
        explicit Random(std::uint64_t seed) : m_generator(seed) {}

        void SetSeed(std::uint64_t seed)
        {
            m_generator.seed(seed);
        }

        template<std::integral T = int>
        T Int(T min, T max)
        {
            std::uniform_int_distribution<T> distribution(min, max);
            return distribution(m_generator);
        }

        template<std::floating_point T = float>
        T Float(T min = 0, T max = 1)
        {
            std::uniform_real_distribution<T> distribution(min, max);
            return distribution(m_generator);
        }

        /**
         * \brief 单位半径球面均匀采样
         * \return 球面上的点坐标
         */
        template<std::floating_point T = float>
        Vector3<T> UniformSampleOnSphere()
        {
            T zeta1 = Float<T>();
            T zeta2 = Float<T>();
            T cosTheta = 1 - 2 * zeta1;
            T sinTheta = std::sqrt(1 - cosTheta * cosTheta);
            T phi = 2 * std::numbers::pi * zeta2;
            return { sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta };
        }

        /**
         * \brief 单位半径半球面均匀采样
         * \return 球面上的点坐标
         */
        template<std::floating_point T = float>
        Vector3<T> UniformSampleOnHemisphere()
        {
            // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations
            T zeta1 = Float<T>();
            T zeta2 = Float<T>();
            T sinTheta = std::sqrt(1 - zeta1 * zeta1);
            T phi = 2 * std::numbers::pi_v<T> * zeta2;
            return { sinTheta * std::cos(phi), sinTheta * std::sin(phi), zeta1 };
        }

        template<std::floating_point T = float>
        Vector3<T> CosineWeightedSampleOnHemisphere()
        {
            T zeta1 = Float<T>();
            T zeta2 = Float<T>();
            T sinTheta = std::sqrt(zeta1);
            T phi = 2 * std::numbers::pi_v<T> * zeta2;
            return { sinTheta * std::cos(phi), sinTheta * std::sin(phi), std::sqrt(1 - zeta1) };
        }

    private:
        std::mt19937_64 m_generator;
    };

    inline Random g_random;
}
