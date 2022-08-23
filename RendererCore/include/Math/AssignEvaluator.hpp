/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

// 赋值类
template<typename DstXprType, typename SrcXprType, typename Functor>
struct Assignment;

template<typename Dst, typename Src>
void call_assignment(Dst& dst, const Src& src)
{
	call_assignment(dst, src, assign_op<typename Dst::Scalar, typename Src::Scalar>());
}

template<typename Dst, typename Src, typename Func>
void call_assignment(Dst& dst, const Src& src, const Func& func, std::enable_if_t<!evaluator_assume_aliasing<Src>::value, void*> = 0)
{
	call_assignment_no_alias(dst, src, func);
}

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

	Assignment<ActualDstTypeCleaned, Src, Func>::run(actualDst, src, func);
}

template<typename DstXprType, typename SrcXprType, typename Functor>
struct Alignment
{
	static void run(DstXprType& dst, const SrcXprType& src, const Functor& func)
	{

	}
};

}