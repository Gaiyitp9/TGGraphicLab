/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math
{
    /**
     * \brief 旋转基类
     * \tparam Derived 旋转表达式
     * \tparam Dimension 维度
     */
    template<typename Derived, int Dimension>
    class RotationBase
    {
        using Scalar = Traits<Derived>::Scalar;

    public:
        using RotationMatrix =  Matrix<Scalar, Dimension, Dimension>;

        const Derived& Expression() const { return *static_cast<Derived const*>(this); }
        Derived& Expression() { return *static_cast<Derived*>(this); }

        RotationMatrix ToRotationMatrix() const
        {
            return Expression().ToRotationMatrix();
        }

        Derived Inverse() const
        {
            return Expression().Inverse();
        }
    };
}
