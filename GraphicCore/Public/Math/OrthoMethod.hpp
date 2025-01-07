/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"

namespace TG::Math
{
    template<typename Derived, typename OtherDerived, std::size_t Size = Traits<Derived>::Size> requires (
        (Traits<Derived>::Rows == 1 || Traits<Derived>::Columns == 1) &&
        (Traits<OtherDerived>::Rows == 1 || Traits<OtherDerived>::Columns == 1) &&
        (Traits<Derived>::Size == Traits<OtherDerived>::Size) &&
        Size == 2 || Size == 3)
    class CrossImpl
    {
        using Matrix = PlainMatrix<Derived>;
        using Scalar = Traits<Derived>::Scalar;
        static constexpr bool LhsIsVector = Traits<Derived>::Rows == 1 || Traits<Derived>::Columns == 1;
        static constexpr bool RhsIsVector = Traits<OtherDerived>::Rows == 1 || Traits<OtherDerived>::Columns == 1;

    public:
        using ReturnType = std::conditional_t<Size == 3, Matrix, Scalar>;

        Matrix operator()(const Derived& lhsXpr, const OtherDerived& rhsXpr) const
            requires (Size == 3)
        {
            Matrix lhsMatrix(lhsXpr);
            Matrix rhsMatrix(rhsXpr);
            return {
                lhsMatrix[1] * rhsMatrix[2] - lhsMatrix[2] * rhsMatrix[1],
                lhsMatrix[2] * rhsMatrix[0] - lhsMatrix[0] * rhsMatrix[2],
                lhsMatrix[0] * rhsMatrix[1] - lhsMatrix[1] * rhsMatrix[0]
            };
        }

        Scalar operator()(const Derived& lhsXpr, const OtherDerived& rhsXpr) const
            requires (Size == 2)
        {
            Matrix lhsMatrix(lhsXpr);
            Matrix rhsMatrix(rhsXpr);
            return lhsMatrix[0] * rhsMatrix[1] - lhsMatrix[1] * rhsMatrix[0];
        }
    };
}
