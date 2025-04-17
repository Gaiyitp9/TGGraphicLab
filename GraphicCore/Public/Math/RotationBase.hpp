/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
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
