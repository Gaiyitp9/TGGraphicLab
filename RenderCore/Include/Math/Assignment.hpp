/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Scalar>
    struct AssignOp
    {
        void AssignCoefficient(Scalar& a, Scalar b) { a = b; }
    };

	// 直接展开赋值
	template<typename DstEvaluator, typename SrcEvaluator, typename Functor, int Index, int Stop>
	struct UnrollAssign
	{
		static void Run(DstEvaluator& dst, const SrcEvaluator& src, Functor functor)
		{
            functor.AssignCoefficient(dst.CoefficientRef(Index), src.Coefficient(Index));
            UnrollAssign<DstEvaluator, SrcEvaluator, Functor, Index + 1, Stop>::Run(dst, src, functor);
		}
	};
	template<typename DstEvaluator, typename SrcEvaluator, typename Functor, int Stop>
	struct UnrollAssign<DstEvaluator, SrcEvaluator, Functor, Stop, Stop>
	{
		static void Run(DstEvaluator&, const SrcEvaluator&, Functor){}
	};

	template<typename DstEvaluator, typename SrcEvaluator, typename Functor, int Size>
	struct Assignment
	{
		static void Run(DstEvaluator& dst, const SrcEvaluator& src, Functor functor = {})
		{
            UnrollAssign<DstEvaluator, SrcEvaluator, Functor, 0, Size>::Run(dst, src, functor);
		}
	};

    template<typename Dst, typename Src>
    inline void CallAssignmentNoAlias(Dst& dst, const Src& src)
    {
        using DstEvaluator = Evaluator<Dst>;
        using SrcEvaluator = Evaluator<Src>;
        using AssignOp = AssignOp<typename Traits<Dst>::Scalar>;

        DstEvaluator dstEvaluator{dst};
        SrcEvaluator srcEvaluator{src};

        Assignment<DstEvaluator, SrcEvaluator, AssignOp, Traits<Dst>::Size>::Run(dstEvaluator, srcEvaluator);
    }

    template<typename Dst, typename Src>
    inline void CallAssignment(Dst& dst, const Src& src)
    {
        typename PlainMatrixType<Src>::Type temp(src);
        CallAssignmentNoAlias(dst, temp);
    }
}
