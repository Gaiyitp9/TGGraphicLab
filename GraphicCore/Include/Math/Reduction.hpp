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
        static constexpr Traversal TraverseManner = HasFlag<Xpr, XprFlag::LinearAccess> ?
            Traversal::Linear : Traversal::Default;

    public:
        Reduction(const Xpr& xpr, ReductionFunctor functor) : m_evaluator(xpr), m_functor(functor) {}

        template<std::size_t Index, std::size_t Length> requires (TraverseManner == Traversal::Default)
        Traits<Xpr>::Scalar Run()
        {
            if constexpr (Length == 0)
                return {};
            else if constexpr (Length == 1)
            {
                constexpr std::size_t row = Index / Traits<Xpr>::Columns;
                constexpr std::size_t column = Index % Traits<Xpr>::Columns;
                return m_evaluator.Entry(row, column);
            }
            else
            {
                constexpr std::size_t halfLength = Length / 2;
                return m_functor(Run<Index, halfLength>(), Run<Index + halfLength, Length - halfLength>());
            }
        }

        template<std::size_t Index, std::size_t Length> requires (TraverseManner == Traversal::Linear)
        Traits<Xpr>::Scalar Run()
        {
            if constexpr (Length == 0)
                return {};
            else if constexpr (Length == 1)
                return m_evaluator.Entry(Index);
            else
            {
                constexpr std::size_t halfLength = Length / 2;
                return m_functor(Run<Index, halfLength>(), Run<Index + halfLength, Length - halfLength>());
            }
        }

    private:
        ConstEvaluator<Xpr> m_evaluator;
        ReductionFunctor m_functor;
    };

    template<typename Xpr, typename ReductionFunctor>
    Traits<Xpr>::Scalar CallReduction(const Xpr& xpr, ReductionFunctor functor)
    {
        Reduction reduction{xpr, functor};
        return reduction.template Run<0, Traits<Xpr>::Size>();
    }
}
