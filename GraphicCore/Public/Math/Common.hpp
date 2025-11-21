/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Core.hpp"

namespace TG::Math
{
    /**
     * \brief 计算反射向量
     * \param v 入射光线
     * \param n 法线
     * \return 反射光线
     */
    template<typename T = float>
    Vector3<T> Reflect(const Vector3<T>& v, const Vector3<T>& n)
    {
        return v - static_cast<T>(2.0) * v.Dot(n) * n;
    }

    /**
     * \brief 计算折射向量
     * \param r 入射光线
     * \param n 法线
     * \param refractionRatio 入射介质折射率与出射介质折射率比值
     * \return 折射光线
     */
    template<typename T = float>
    Vector3<T> Refract(const Vector3<T>& r, const Vector3<T>& n, T refractionRatio)
    {
        T cosTheta = std::min(-r.Dot(n), static_cast<T>(1.0));
        Vector3<T> rOutPerp = refractionRatio * (r + cosTheta * n);
        Vector3<T> rOutParallel = -std::sqrt(1.0f - rOutPerp.SquaredNorm()) * n;
        return rOutPerp + rOutParallel;
    }
}
