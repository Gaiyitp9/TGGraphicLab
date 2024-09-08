/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"

namespace TG::Math
{
    // 赋值运算符
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a = b; }
    struct AssignOp
    {
        void operator()(Scalar& a, Scalar b) const { a = b; }
    };
    // 赋值运算算法，全部封装在一个类中，方便管理
    template<typename Dst, typename Src, typename AssignFunctor>
    class Assignment
    {
    	static constexpr Traversal TraverseMethod = HasFlag<Dst, XprFlag::LinearAccess> &&
    	    HasFlag<Src, XprFlag::LinearAccess> && HasFlag<Dst, XprFlag::RowMajor> == HasFlag<Src, XprFlag::RowMajor>
                ? Traversal::Linear : Traversal::Default;

    public:
		// 通过行列赋值
        void operator()(Dst& dst, const Src& src, AssignFunctor functor) const
            requires (TraverseMethod == Traversal::Default)
        {
            RefEvaluator<Dst> dstEvaluator{dst};
            Evaluator<Src> srcEvaluator{src};

            for (int i = 0; i < Traits<Dst>::Rows; ++i)
                for (int j = 0; j < Traits<Src>::Columns; ++j)
				    functor(dstEvaluator.Entry(i, j), srcEvaluator.Entry(i, j));
        }
    	// 线性赋值
        void operator()(Dst& dst, const Src& src, AssignFunctor functor) const
            requires (TraverseMethod == Traversal::Linear)
        {
            RefEvaluator<Dst> dstEvaluator{dst};
            Evaluator<Src> srcEvaluator{src};

            for (int i = 0; i < Traits<Dst>::Size; ++i)
				functor(dstEvaluator.Entry(i), srcEvaluator.Entry(i));
        }
    };

    // 矩阵表达式赋值概念，目标表达式和源表达式的行列数需要相等，且表达式需要是左值
    template<typename Dst, typename Src>
    concept Assignable = Traits<Dst>::Rows == Traits<Src>::Rows && Traits<Dst>::Columns == Traits<Src>::Columns &&
            HasFlag<Dst, XprFlag::LeftValue>;
    // 求值器是否需要考虑Aliasing
    template<typename T> constexpr bool EvaluatorAssumeAliasing = false;

    // In Eigen, aliasing refers to assignment statement in which the same matrix (or array or vector)
    // appears on the left and on the right of the assignment operators.
    template<typename Dst, typename Src, typename AssignFunctor = AssignOp<typename Traits<Dst>::Scalar>>
        requires Assignable<Dst, Src>
    void CallAssignmentNoAlias(Dst& dst, const Src& src, const AssignFunctor& func = {})
    {
    	Assignment<Dst, Src, AssignFunctor>{}(dst, src, func);
    }

    template<typename Dst, typename Src, typename AssignFunctor> requires EvaluatorAssumeAliasing<Src>
    void CallAssignment(Dst& dst, const Src& src, const AssignFunctor& func)
    {
        PlainMatrix<Src> temp{src};
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
