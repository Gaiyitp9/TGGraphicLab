/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Evaluator, typename Functor, std::size_t Index, std::size_t Length>
    struct UnrollDefaultRedux
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr std::size_t HalfLength = Length / 2;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return functor(UnrollDefaultRedux<Evaluator, Functor, Index, HalfLength>::Run(evaluator, functor),
                           UnrollDefaultRedux<Evaluator, Functor, Index + HalfLength, Length - HalfLength>::Run(evaluator, functor));
        }
    };
    template<typename Evaluator, typename Functor, std::size_t Index>
    struct UnrollDefaultRedux<Evaluator, Functor, Index, 1>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr std::size_t Row = Index / Traits<typename Evaluator::XprType>::Columns;
        static constexpr std::size_t Column = Index % Traits<typename Evaluator::XprType>::Columns;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return evaluator.Coefficient(Row, Column);
        }
    };
    template<typename Evaluator, typename Functor, std::size_t Index>
    struct UnrollDefaultRedux<Evaluator, Functor, Index, 0>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;

        static Scalar Run(const Evaluator& evaluator, Functor functor) { return {}; }
    };

    template<typename Evaluator, typename Functor, std::size_t Index, std::size_t Length>
    struct UnrollLinearRedux
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr std::size_t HalfLength = Length / 2;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return functor(UnrollLinearRedux<Evaluator, Functor, Index, HalfLength>::Run(evaluator, functor),
                           UnrollLinearRedux<Evaluator, Functor, Index + HalfLength, Length - HalfLength>::Run(evaluator, functor));
        }
    };
    template<typename Evaluator, typename Functor, std::size_t Index>
    struct UnrollLinearRedux<Evaluator, Functor, Index, 1>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return evaluator.Coefficient(Index);
        }
    };
    template<typename Evaluator, typename Functor, std::size_t Index>
    struct UnrollLinearRedux<Evaluator, Functor, Index, 0>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;

        static Scalar Run(const Evaluator& evaluator, Functor functor) { return {}; }
    };

    template<typename Evaluator, typename Functor, TraversalType Traversal =
        ContainFlag<typename Evaluator::XprType, XprFlag::LinearAccess>() ?
        TraversalType::Linear : TraversalType::Default>
    struct Redux;

    template<typename Evaluator, typename Functor>
    struct Redux<Evaluator, Functor, TraversalType::Default>
    {
        using ReturnType = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr std::size_t Size = Traits<typename Evaluator::XprType>::Size;

        static ReturnType Run(const Evaluator& evaluator, Functor functor)
        {
            return UnrollDefaultRedux<Evaluator, Functor, 0, Size>::Run(evaluator, functor);
        }
    };

    template<typename Evaluator, typename Functor>
    struct Redux<Evaluator, Functor, TraversalType::Linear>
    {
        using ReturnType = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr std::size_t Size = Traits<typename Evaluator::XprType>::Size;

        static ReturnType Run(const Evaluator& evaluator, Functor functor)
        {
            return UnrollLinearRedux<Evaluator, Functor, 0, Size>::Run(evaluator, functor);
        }
    };

    template<typename Xpr, typename Functor> requires MatrixExpression<Xpr>
    Traits<Xpr>::Scalar CallRedux(const Xpr& xpr, Functor functor)
    {
        Evaluator<Xpr> evaluator{xpr};

        return Redux<Evaluator<Xpr>, Functor>::Run(evaluator, functor);
    }
}
