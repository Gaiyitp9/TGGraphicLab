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
        Reduction(const Xpr& xpr, ReductionFunctor functor) : m_evaluator(xpr), m_functor(functor) {}

        Scalar Run() requires (TraverseMethod == Traversal::Default)
        {
            Scalar result = m_evaluator.Entry(0, 0);

            for (int j = 1; j < Traits<Xpr>::Columns; ++j)
                result = m_functor(result, m_evaluator.Entry(0, j));

            for (int i = 1; i < Traits<Xpr>::Rows; ++i)
                for (int j = 0; j < Traits<Xpr>::Columns; ++j)
                    result = m_functor(result, m_evaluator.Entry(i, j));

            return result;
        }

        Scalar Run() requires (TraverseMethod == Traversal::Linear)
        {
            Scalar result = m_evaluator.Entry(0);

            for (int i = 1; i < Traits<Xpr>::Size; ++i)
                result = m_functor(result, m_evaluator.Entry(i));

            return result;
        }

    private:
        Evaluator<Xpr> m_evaluator;
        ReductionFunctor m_functor;
    };
}
