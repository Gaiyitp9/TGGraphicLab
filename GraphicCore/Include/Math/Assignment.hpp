/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"

namespace TG::Math
{
    // 赋值运算
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a = b; }
    struct AssignOp
    {
        void operator()(Scalar& a, Scalar b) { a = b; }
    };

    template<typename Dst, typename Src, typename AssignFunctor>
    class Assignment
    {
    	static constexpr Traversal TraverseManner = HasFlag<Dst, XprFlag::LinearAccess> &&
    	    HasFlag<Src, XprFlag::LinearAccess> && HasFlag<Dst, XprFlag::RowMajor> == HasFlag<Src, XprFlag::RowMajor>
                ? Traversal::Linear : Traversal::Default;

    public:
        Assignment(Dst& dst, const Src& src, AssignFunctor functor)
            : m_dstEvaluator(dst), m_srcEvaluator(src), m_functor(functor)
        {}

		// 通过行列赋值
        void Run() requires (TraverseManner == Traversal::Default)
        {
            for (int i = 0; i < Traits<Dst>::Rows; ++i)
                for (int j = 0; j < Traits<Src>::Columns; ++j)
				    m_functor(m_dstEvaluator.Entry(i, j), m_srcEvaluator.Entry(i, j));
        }
    	// 线性赋值
        void Run() requires (TraverseManner == Traversal::Linear)
        {
            for (int i = 0; i < Traits<Dst>::Size; ++i)
				m_functor(m_dstEvaluator.Entry(i), m_srcEvaluator.Entry(i));
        }

    private:
        RefEvaluator<Dst> m_dstEvaluator;
        Evaluator<Src> m_srcEvaluator;
        AssignFunctor m_functor;
    };

    // 矩阵表达式赋值概念，目标表达式和源表达式的行列数需要相等，且表达式需要是左值
    template<typename Dst, typename Src>
    concept Assignable = Traits<Dst>::Rows == Traits<Src>::Rows && Traits<Dst>::Columns == Traits<Src>::Columns &&
            HasFlag<Dst, XprFlag::LeftValue>;

    template <typename T> constexpr bool EvaluatorAssumeAliasing  = false;

    // In Eigen, aliasing refers to assignment statement in which the same matrix (or array or vector)
    // appears on the left and on the right of the assignment operators.
    template<typename Dst, typename Src, typename AssignFunctor = AssignOp<typename Traits<Dst>::Scalar>>
        requires Assignable<Dst, Src>
    void CallAssignmentNoAlias(Dst& dst, const Src& src, const AssignFunctor& func = {})
    {
    	Assignment assign(dst, src, func);
    	assign.Run();
    }

    template<typename Dst, typename Src, typename AssignFunctor> requires EvaluatorAssumeAliasing<Src>
    void CallAssignment(Dst& dst, const Src& src, const AssignFunctor& func)
    {
        typename PlainMatrixType<Src>::Type temp(src);
        CallAssignmentNoAlias(dst, temp, func);
    }

    template<typename Dst, typename Src, typename AssignFunctor> requires !EvaluatorAssumeAliasing<Src>
    void CallAssignment(Dst& dst, const Src& src, const AssignFunctor& func)
    {
        CallAssignmentNoAlias(dst, src, func);
    }

    template<typename Dst, typename Src>
    void CallAssignment(Dst& dst, const Src& src)
    {
        CallAssignment(dst, src, AssignOp<typename Traits<Dst>::Scalar>{});
    }
}
