/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Evaluator, typename Functor, int Start, int Length>
    struct UnrollRedux
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;
        static constexpr int HalfLength = Length / 2;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return functor(UnrollRedux<Evaluator, Functor, Start, HalfLength>::Run(evaluator, functor),
                           UnrollRedux<Evaluator, Functor, Start + HalfLength, Length - HalfLength>::Run(evaluator, functor));
        }
    };

    template<typename Evaluator, typename Functor, int Start>
    struct UnrollRedux<Evaluator, Functor, Start, 1>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;

        static Scalar Run(const Evaluator& evaluator, Functor functor)
        {
            return evaluator.Coefficient(Start);
        }
    };

    template<typename Evaluator, typename Functor, int Start>
    struct UnrollRedux<Evaluator, Functor, Start, 0>
    {
        using Scalar = Traits<typename Evaluator::XprType>::Scalar;

        static Scalar Run(const Evaluator& evaluator, Functor functor) { return {}; }
    };

    template<typename Xpr, typename Functor>
    inline Traits<Xpr>::Scalar CallRedux(const Xpr& xpr, Functor functor)
    {
        Evaluator<Xpr> evaluator{xpr};

        return UnrollRedux<Evaluator<Xpr>, Functor, 0, Traits<Xpr>::Size>::Run(evaluator, functor);
    }
}
