/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"

namespace TG::Math
{
    template<typename Xpr, typename ReductionFunctor>
    class Reduction
    {
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr Traversal TraverseMethod = HasFlag<Xpr, XprFlag::LinearAccess> ?
            Traversal::Linear : Traversal::Default;

    public:
        Scalar operator()(const Xpr& xpr, ReductionFunctor functor) const requires (TraverseMethod == Traversal::Default)
        {
            Evaluator<Xpr> evaluator{xpr};
            Scalar result = evaluator.Entry(0, 0);

            for (int j = 1; j < Traits<Xpr>::Columns; ++j)
                result = functor(result, evaluator.Entry(0, j));

            for (int i = 1; i < Traits<Xpr>::Rows; ++i)
                for (int j = 0; j < Traits<Xpr>::Columns; ++j)
                    result = functor(result, evaluator.Entry(i, j));

            return result;
        }

        Scalar operator()(const Xpr& xpr, ReductionFunctor functor) const requires (TraverseMethod == Traversal::Linear)
        {
            Evaluator<Xpr> evaluator{xpr};
            Scalar result = evaluator.Entry(0);

            for (int i = 1; i < Traits<Xpr>::Size; ++i)
                result = functor(result, evaluator.Entry(i));

            return result;
        }
    };
}
