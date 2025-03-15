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
        using RotationMatrix =  Matrix<Scalar, Dimension, Dimension>;

    public:
        RotationMatrix ToRotationMatrix() const
        {

        }
    };
}
