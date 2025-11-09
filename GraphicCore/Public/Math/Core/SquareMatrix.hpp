/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Derived>
    class SquareMatrix : public MatrixBase<Derived>
    {
        using Scalar = Traits<Derived>::Scalar;

    public:
        Scalar Determinant() const
        {
            return DeterminantImpl<Derived, Traits<Derived>::Rows>{}(this->Expression());
        }

        Math::Inverse<Derived> Inverse() const
        {
            return Math::Inverse<Derived>(this->Expression());
        }
    };
}
