/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    // 赋值遍历类型
    enum class TraversalType : char
    {
        Default,
        Linear,
        Vectorized,
    };

    // 赋值运算
    template<typename Scalar>
    struct AssignOp
    {
        void AssignCoefficient(Scalar& a, Scalar b) { a = b; }
    };

	// 直接展开赋值
    // 线性赋值
	template<typename Kernel, int Index, int Stop>
	struct UnrollLinearAssign
	{
		static void Run(Kernel& kernel)
		{
            kernel.AssignCoefficient(Index);
            UnrollLinearAssign<Kernel, Index + 1, Stop>::Run(kernel);
		}
	};
	template<typename Kernel, int Stop>
	struct UnrollLinearAssign<Kernel, Stop, Stop>
	{
		static void Run(Kernel&){}
	};

    // 通过行列赋值
    template<typename Kernel, int Index, int Stop>
    struct UnrollDefaultAssign
    {
        static constexpr int Row = Index / Kernel::DstTraits::Columns;
        static constexpr int Column = Index % Kernel::DstTraits::Columns;

        static void Run(Kernel& kernel)
        {
            kernel.AssignCoefficient(Row, Column);
            UnrollDefaultAssign<Kernel, Index + 1, Stop>::Run(kernel);
        }
    };
    template<typename Kernel, int Stop>
    struct UnrollDefaultAssign<Kernel, Stop, Stop>
    {
        static void Run(Kernel&){}
    };

    // 赋值核，封装赋值过程涉及的细节
    template<typename DstEvaluator, typename SrcEvaluator, typename Functor>
    class AssignmentKernel
    {
    public:
        using DstTraits = Traits<typename DstEvaluator::XprType>;
        using SrcTraits = Traits<typename SrcEvaluator::XprType>;

        AssignmentKernel(DstEvaluator& dstEvaluator, const SrcEvaluator& srcEvaluator, Functor functor)
            : m_dstEvaluator(dstEvaluator), m_srcEvaluator(srcEvaluator), m_functor(functor) {}

        void AssignCoefficient(int index)
        {
            m_functor.AssignCoefficient(m_dstEvaluator.CoefficientRef(index), m_srcEvaluator.Coefficient(index));
        }

        void AssignCoefficient(int row, int column)
        {
            m_functor.AssignCoefficient(m_dstEvaluator.CoefficientRef(row, column), m_srcEvaluator.Coefficient(row, column));
        }

    private:
        DstEvaluator& m_dstEvaluator;
        const SrcEvaluator& m_srcEvaluator;
        Functor m_functor;
    };

    template<typename Kernel, TraversalType Traversal =
            (Kernel::DstTraits::Flags & Kernel::SrcTraits::Flags & XprFlag::LinearAccess) != XprFlag::None ?
            TraversalType::Linear : TraversalType::Default>
    struct Assignment;

    template<typename Kernel>
    struct Assignment<Kernel, TraversalType::Default>
    {
        static void Run(Kernel& kernel)
        {
            UnrollDefaultAssign<Kernel, 0, Kernel::DstTraits::Size>::Run(kernel);
        }
    };

	template<typename Kernel>
	struct Assignment<Kernel, TraversalType::Linear>
	{
		static void Run(Kernel& kernel)
		{
            UnrollLinearAssign<Kernel, 0, Kernel::DstTraits::Size>::Run(kernel);
		}
	};

    template<typename Dst, typename Src>
    inline void CallAssignmentNoAlias(Dst& dst, const Src& src)
    {
        using DstEvaluator = Evaluator<Dst>;
        using SrcEvaluator = Evaluator<Src>;
        using AssignOp = AssignOp<typename Traits<Dst>::Scalar>;
        using Kernel = AssignmentKernel<DstEvaluator, SrcEvaluator, AssignOp>;

        DstEvaluator dstEvaluator{dst};
        SrcEvaluator srcEvaluator{src};

        Kernel kernel(dstEvaluator, srcEvaluator, AssignOp{});
        Assignment<Kernel>::Run(kernel);
    }

    template<typename Dst, typename Src>
    inline void CallAssignment(Dst& dst, const Src& src)
    {
        typename PlainMatrixType<Src>::Type temp(src);
        CallAssignmentNoAlias(dst, temp);
    }
}
