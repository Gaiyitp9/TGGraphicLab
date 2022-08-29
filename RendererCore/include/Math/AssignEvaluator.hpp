/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename DstEvaluator, typename SrcEvaluator, typename AssignFunc, int MaxPacketSize = -1>
struct copy_using_evaluator_traits
{
	using Dst = DstEvaluator::XprType;
	using DstScalar = Dst::Scalar;

	constexpr static int DstFlags = DstEvaluator::Flags;
	constexpr static int SrcFlags = SrcEvaluator::Flags;

	constexpr static int DstAlignment = DstEvaluator::Alignment;
	constexpr static int SrcAlignment = SrcEvaluator::Alignment;
	constexpr static bool DstHasDirectAccess = (DstFlags & DirectAccessBit) == DirectAccessBit;
	constexpr static int JointAlignment = DstAlignment < SrcAlignment ? DstAlignment : SrcAlignment;

	constexpr static int InnerSize = Dst::IsVectorAtCompile ? Dst::SizeAtCompileTime :
									 DstFlags & RowMajorBit ? Dst::ColsAtCompileTime : Dst::RowAtCompileTime;
	constexpr static int OuterStride = outer_stride_at_compile_time<Dst>::ret;
};

// 赋值类
template<typename DstXprType, typename SrcXprType, typename Functor>
struct Assignment;

template<typename Dst, typename Src, typename Func>
void call_assignment_no_alias(Dst& dst, const Src& src, const Func& func)
{
	constexpr static int NeedToTranspose =
		(Dst::RowsAtCompileTime == 1 && Src::ColsAtCompileTime == 1) ||
		(Dst::ColsAtCompileTime == 1 && Src::RowsAtCompileTime == 1) &&
		Dst::SizeAtCompileTime != 1;

	using ActualDstTypeCleaned = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst>;
	using ActualDstType = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst&>;
	ActualDstType actualDst(dst);

	static_assert(is_lvalue(Dst), "The expression is not a left value.");

	Assignment<ActualDstTypeCleaned, Src, Func>::run(actualDst, src, func);
}

template<typename Dst, typename Src, typename Func>
void call_assignment(Dst& dst, Src& src, const Func& func)
{
	using type = Matrix<typename traits<Src>::Scalar,
		traits<Src>::RowsAtCompileTime,
		traits<Src>::ColsAtCompileTime,
		traits<Src>::Flags & RowMajorBit ? StorageOption::RowMajor : StorageOption::ColMajor
	>;

	if constexpr (evaluator_assume_aliasing<Src>::value)
	{
		type tmp(src);
		call_assignment_no_alias(dst, tmp, func);
	}
	else 
		call_assignment_no_alias(dst, src, func);
}

template<typename Dst, typename Src>
void call_assignment(Dst& dst, const Src& src)
{
	call_assignment(dst, src, assign_op<typename Dst::Scalar, typename Src::Scalar>());
}

template<typename Kernel,
		 int Traversal = Kernel::AssignmentTraits::Traversal,
		 int Unrolling = Kernel::AssignmentTraits::Unrolling>
struct assignment_loop;

template<typename DstEvaluatorTypeT, typename SrcEvaluatorTypeT, typename Functor>
class generic_assignment_kernel
{
protected:
	using DstXprType = DstEvaluatorTypeT::XprType;
	using SrcXprType = SrcEvaluatorTypeT::XprType;

public:
	using DstEvaluatorType = DstEvaluatorTypeT;
	using SrcEvaluatorType = SrcEvaluatorTypeT;
	using Scalar = DstEvaluatorType::Scalar;
	using AssignmentTraits = copy_using_evaluator_traits<DstEvaluatorType, SrcEvaluatorType, Functor>;
	using PacketType = AssignmentTraits::PacketType;

	generic_assignment_kernel(DstEvaluatorType& dst, const SrcEvaluatorType& src, const Functor& func, DstXprType& dstExpr)
		: m_dst(dst), m_src(src), m_functor(func), m_dstExpr(dstExpr)
	{}

protected:
	DstEvaluatorType& m_dst;
	const SrcEvaluatorType& m_src;
	const Functor& m_functor;
	DstXprType& m_dstExpr;
};

template<typename DstXprType, typename SrcXprType, typename Functor>
struct Assignment
{
	static void run(DstXprType& dst, const SrcXprType& src, const Functor& func)
	{
		using DstEvaluatorType = evaluator<DstXprType>;
		using SrcEvaluatorType = evaluator<SrcXprType>;
		SrcEvaluatorType srcEvaluator(src);
		DstEvaluatorType dstEvaluator(dst);

		using Kernel = generic_assignment_kernel<DstEvaluatorType, SrcEvaluatorType, Functor>;
		Kernel kernel(dstEvaluator, srcEvaluator, func, dst.const_cast_derived());

		assignment_loop<Kernel>::run(kernel);
	}
};

}