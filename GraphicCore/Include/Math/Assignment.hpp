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
        template<std::size_t Index> requires (TraverseManner == Traversal::Default)
        void Run()
        {
            if constexpr (Index < Traits<Dst>::Size)
            {
	            constexpr std::size_t row = Index / Traits<Dst>::Columns;
	            constexpr std::size_t column = Index % Traits<Dst>::Columns;
				m_functor(m_dstEvaluator.Entry(row, column), m_srcEvaluator.Entry(row, column));
                Run<Index + 1>();
            }
        }
    	// 线性赋值
        template<std::size_t Index> requires (TraverseManner == Traversal::Linear)
        void Run()
        {
            if constexpr (Index < Traits<Dst>::Size)
            {
				m_functor(m_dstEvaluator.Entry(Index), m_srcEvaluator.Entry(Index));
                Run<Index + 1>();
            }
        }

    private:
        Evaluator<Dst> m_dstEvaluator;
        ConstEvaluator<Src> m_srcEvaluator;
        AssignFunctor m_functor;
    };

    // 矩阵表达式赋值概念，目标表达式和源表达式的行列数需要相等，且表达式需要是左值
    template<typename Dst, typename Src>
    concept Assignable = Traits<Dst>::Rows == Traits<Src>::Rows && Traits<Dst>::Columns == Traits<Src>::Columns &&
            HasFlag<Dst, XprFlag::LeftValue>;

    // In Eigen, aliasing refers to assignment statement in which the same matrix (or array or vector)
    // appears on the left and on the right of the assignment operators.
    template<typename Dst, typename Src, typename AssignFunctor = AssignOp<typename Traits<Dst>::Scalar>>
        requires Assignable<Dst, Src>
    void CallAssignmentNoAlias(Dst& dst, const Src& src, const AssignFunctor& func = {})
    {
    	Assignment assign(dst, src, func);
    	assign.template Run<0>();
    }

    template<typename Dst, typename Src, typename AssignFunctor = AssignOp<typename Traits<Dst>::Scalar>>
    void CallAssignment(Dst& dst, const Src& src, const AssignFunctor& func = {})
    {
        typename PlainMatrixType<Src>::Type temp(src);
        CallAssignmentNoAlias(dst, temp, func);
    }
}
