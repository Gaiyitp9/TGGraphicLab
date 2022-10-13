/************************************************************************
* This file is part of Eigen, a lightweight C++ template library		*
* for linear algebra.													*
*																		*
* Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>			*
* Copyright (C) 2011-2014 Gael Guennebaud <gael.guennebaud@inria.fr>	*
* Copyright (C) 2011-2012 Jitse Niesen <jitse@maths.leeds.ac.uk>		*
*																		*
* This Source Code Form is subject to the terms of the Mozilla			*
* Public License v. 2.0. If a copy of the MPL was not distributed		*
* with this file, You can obtain one at http://mozilla.org/MPL/2.0/.	*
*																		*
* Noted: I made some modifications in this file.						*
*************************************************************************/
#pragma once

namespace LCH::Math
{

template<typename DstEvaluator, typename SrcEvaluator, typename AssignFunc>
struct copy_using_evaluator_traits
{
	using Dst = DstEvaluator::XprType;
	using DstScalar = Dst::Scalar;

	constexpr static Flag DstFlags = DstEvaluator::Flags;
	constexpr static Flag SrcFlags = SrcEvaluator::Flags;

	constexpr static int DstAlignment = DstEvaluator::Alignment;
	constexpr static int SrcAlignment = SrcEvaluator::Alignment;
	constexpr static bool DstIsRowMajor = not_none(DstFlags & Flag::RowMajor);
	constexpr static bool SrcIsRowMajor = not_none(SrcFlags & Flag::RowMajor);
	constexpr static bool StorageOrdersAgree = DstIsRowMajor == SrcIsRowMajor;

	constexpr static int InnerSize = DstIsRowMajor ? Dst::ColsAtCompileTime : Dst::RowsAtCompileTime;
	constexpr static int LinearSize = Dst::SizeAtCompileTime;
	using LinearPacketType = best_packet<DstScalar, LinearSize>;
	using InnerPacketType = best_packet<DstScalar, InnerSize>;
	constexpr static int LinearPacketSize = unpacket_traits<LinearPacketType>::Size;
	constexpr static int InnerPacketSize = unpacket_traits<InnerPacketType>::Size;

	constexpr static bool MayVectorize = StorageOrdersAgree && not_none(DstFlags & SrcFlags & ActualPacketAccess);
	constexpr static bool MayLinearize = StorageOrdersAgree && not_none(DstFlags & SrcFlags & Flag::LinearAccess);
	constexpr static bool MayInnerVectorize = MayVectorize && InnerSize != Dynamic && InnerSize % InnerPacketSize == 0;
	constexpr static bool MayLinearVectorize = MayVectorize && MayLinearize;

	constexpr static TraversalType Traversal = MayLinearVectorize && (LinearPacketSize >= InnerPacketSize) ? TraversalType::LinearVectorized
									: MayInnerVectorize ? TraversalType::InnerVectorized
									: MayLinearVectorize ? TraversalType::LinearVectorized
									: MayLinearize ? TraversalType::Linear : TraversalType::Default;
	constexpr static bool Vectorized = Traversal == TraversalType::InnerVectorized ||
									Traversal == TraversalType::LinearVectorized;

	using PacketType = std::conditional_t<Traversal == TraversalType::LinearVectorized, LinearPacketType, InnerPacketType>;

	constexpr static int ActualPacketSize = Traversal == TraversalType::LinearVectorized ? LinearPacketSize
									: Vectorized ? InnerPacketSize : 1;

	constexpr static bool MayUnrollCompletely = LinearSize != Dynamic;
	constexpr static UnrollingType Unrolling = MayUnrollCompletely ? UnrollingType::Complete : UnrollingType::None;
};

template<typename Kernel, int Index, int Stop>
struct DefaultTraversalCompleteUnrolling
{
	using DstXprType = Kernel::DstXprType;
	constexpr static int outer = Index / DstXprType::InnerSizeAtCompileTime;
	constexpr static int inner = Index % DstXprType::InnerSizeAtCompileTime;

	static void Run(Kernel& kernel)
	{
		kernel.AssignCoeffByOuterInner(outer, inner);
		DefaultTraversalCompleteUnrolling<Kernel, Index + 1, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct DefaultTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel, int Index, int Stop>
struct LinearTraversalCompleteUnrolling
{
	static void Run(Kernel& kernel)
	{
		kernel.AssignCoeff(Index);
		LinearTraversalCompleteUnrolling<Kernel, Index + 1, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct LinearTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel, int Index, int Stop>
struct VectorizedTraversalCompleteUnrolling
{
	using DstXprType = Kernel::DstXprType;
	using PacketType = Kernel::PacketType;

	constexpr static int outer = Index / DstXprType::InnerSizeAtCompileTime;
	constexpr static int inner = Index % DstXprType::InnerSizeAtCompileTime;
	constexpr static int SrcAlignment = Kernel::AssignmentTraits::SrcAlignment;
	constexpr static int DstAlignment = Kernel::AssignmentTraits::DstAlignment;

	static void Run(Kernel& kernel)
	{
		kernel.template AssignPacketByOuterInner<DstAlignment, SrcAlignment, PacketType>(outer, inner);
		constexpr static int NextIndex = Index + unpacket_traits<PacketType>::Size;
		VectorizedTraversalCompleteUnrolling<Kernel, NextIndex, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct VectorizedTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel,
	TraversalType Traversal = Kernel::AssignmentTraits::Traversal,
	UnrollingType Unrolling = Kernel::AssignmentTraits::Unrolling>
struct assignment_loop;

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Default, UnrollingType::None>
{
	static void Run(Kernel& kernel)
	{
		for (int outer = 0; outer < kernel.outerSize(); ++outer)
		{
			for (int inner = 0; inner < kernel.innerSize(); ++inner)
			{
				kernel.AssignCoeffByOuterInner(outer, inner);
			}
		}
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Default, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstEvaluatorType::XprType;
		DefaultTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::LinearVectorized, UnrollingType::None>
{
	static void Run(Kernel& kernel)
	{
		using PacketType = Kernel::PacketType;
		constexpr static int requestedAlignment = Kernel::AssignmentTraits::LinearRequiredAlignment;
		constexpr static int packetSize = unpacket_traits<PacketType>::Size;
		constexpr static int dstAlignment = Kernel::AssignmentTraits::DstAlignment;
		constexpr static int srcAlignment = Kernel::AssignmentTraits::SrcAlignment;

		const int size = kernel.size();
		const int alignedSize = (size / packetSize) * packetSize;

		for (int index = 0; index < alignedSize; index += packetSize)
			kernel.template AssignPacket<dstAlignment, srcAlignment, PacketType>(index);

		for (int index = alignedSize; index < size; ++index)
			kernel.AssignCoeff(index);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::LinearVectorized, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		using PacketType = Kernel::PacketType;

		constexpr static int size = DstXprType::SizeAtCompileTime;
		constexpr static int packetSize = unpacket_traits<PacketType>::Size;
		constexpr static int alignedSize = (size / packetSize) * packetSize;

		VectorizedTraversalCompleteUnrolling<Kernel, 0, alignedSize>::Run(kernel);
		DefaultTraversalCompleteUnrolling<Kernel, alignedSize, size>::Run(kernel);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::InnerVectorized, UnrollingType::None>
{
	using PacketType = Kernel::PacketType;
	constexpr static int SrcAlignment = Kernel::AssignmentTraits::SrcAlignment;
	constexpr static int DstAlignment = Kernel::AssignmentTraits::DstAlignment;

	static void Run(Kernel& kernel)
	{
		const int innerSize = kernel.innerSize();
		const int outerSize = kernel.outerSize();
		const int packetSize = unpacket_traits<PacketType>::size;
		for (int outer = 0; outer < outerSize; ++outer)
			for (int inner = 0; inner < innerSize; inner += packetSize)
				kernel.template assignPacketByOuterInner<DstAlignment, SrcAlignment, PacketType>(outer, inner);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::InnerVectorized, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		VectorizedTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Linear, UnrollingType::None>
{
	static void Run(Kernel& kernel)
	{
		const int size = kernel.size();
		for (int i = 0; i < size; ++i)
			kernel.AssignCoeff(i);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Linear, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		LinearTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

template<typename DstEvaluatorTypeT, typename SrcEvaluatorTypeT, typename Functor>
class generic_assignment_kernel
{
public:
	using DstXprType = DstEvaluatorTypeT::XprType;
	using DstEvaluatorType = DstEvaluatorTypeT;
	using SrcEvaluatorType = SrcEvaluatorTypeT;
	using Scalar = DstEvaluatorType::Scalar;
	using AssignmentTraits = copy_using_evaluator_traits<DstEvaluatorType, SrcEvaluatorType, Functor>;
	using PacketType = AssignmentTraits::PacketType;

	generic_assignment_kernel(DstEvaluatorType& dst, const SrcEvaluatorType& src, int size, int outerSize,
		int innerSize, const Functor& func) : m_dst(dst), m_src(src), m_size(size), 
		m_outerSize(outerSize), m_innerSize(innerSize), m_functor(func)
	{}

	constexpr int size() const { return m_size; }
	constexpr int outerSize() const { return m_outerSize; }
	constexpr int innerSize() const { return m_innerSize; }

	void AssignCoeff(int row, int col)
	{
		m_functor.AssignCoeff(m_dst.coeffRef(row, col), m_src.coeff(row, col));
	}

	void AssignCoeff(int index)
	{
		m_functor.AssignCoeff(m_dst.coeffRef(index), m_src.coeff(index));
	}

	void AssignCoeffByOuterInner(int outer, int inner)
	{
		int row = RowIndexByOuterInner(outer, inner);
		int col = ColIndexByOuterInner(outer, inner);
		AssignCoeff(row, col);
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacket(int row, int col)
	{
		m_functor.template AssignPacket<StoreMode>(&m_dst.coeffRef(row, col), m_src.template packet<LoadMode, PacketType>(row, col));
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacket(int index)
	{
		m_functor.template AssignPacket<StoreMode>(&m_dst.coeffRef(index), m_src.template packet<LoadMode, PacketType>(index));
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacketByOuterInner(int outer, int inner)
	{
		int row = RowIndexByOuterInner(outer, inner);
		int col = RowIndexByOuterInner(outer, inner);
		AssignPacket<StoreMode, LoadMode, PacketType>(row, col);
	}

private:
	int RowIndexByOuterInner(int outer, int inner)
	{
		if constexpr (AssignmentTraits::DstIsRowMajor)
			return outer;
		else
			return inner;
	}

	int ColIndexByOuterInner(int outer, int inner)
	{
		if constexpr (AssignmentTraits::DstIsRowMajor)
			return inner;
		else
			return outer;
	}

private:
	DstEvaluatorType& m_dst;
	const SrcEvaluatorType& m_src;
	const Functor& m_functor;
	int m_size;
	int m_outerSize;
	int m_innerSize;
};

template<typename Dst, typename Src>
void call_assignment_no_alias(Dst& dst, const Src& src)
{
	constexpr static bool NeedToTranspose =
		(Dst::RowsAtCompileTime == 1 && Src::ColsAtCompileTime == 1) ||
		(Dst::ColsAtCompileTime == 1 && Src::RowsAtCompileTime == 1) &&
		Dst::SizeAtCompileTime != 1;
	using Functor = assign_op<typename Dst::Scalar, typename Src::Scalar>;
	using ActualDstTypePlain = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst>;
	using ActualDstType = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst&>;
	using DstEvaluatorType = evaluator<ActualDstTypePlain>;
	using SrcEvaluatorType = evaluator<Src>;

	static_assert(is_lvalue<Dst>, "The destination expression is not a left value.");
	static_assert(have_same_matrix_size<ActualDstTypePlain, Src>, "You mix matrices of different sizes.");

	Functor func;
	ActualDstType actualDst(dst);
	SrcEvaluatorType srcEvaluator(src);
	DstEvaluatorType dstEvaluator(actualDst);

	using Kernel = generic_assignment_kernel<DstEvaluatorType, SrcEvaluatorType, Functor>;
	Kernel kernel(dstEvaluator, srcEvaluator, dst.size(), dst.outerSize(), dst.innerSize(), func);

	assignment_loop<Kernel>::Run(kernel);
}

template<typename Dst, typename Src>
void call_assignment(Dst& dst, Src& src)
{
	using type = Matrix<typename traits<Src>::Scalar,
		traits<Src>::RowsAtCompileTime,
		traits<Src>::ColsAtCompileTime,
		not_none(traits<Src>::Flags & Flag::RowMajor) ? StorageOption::RowMajor : StorageOption::ColMajor
	>;

	if constexpr (evaluator_assume_aliasing<Src>)
	{
		type tmp(src);
		call_assignment_no_alias(dst, tmp);
	}
	else
		call_assignment_no_alias(dst, src);
}

}