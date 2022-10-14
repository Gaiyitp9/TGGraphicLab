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

template<typename BinaryOp, typename Lhs, typename Rhs>
struct traits<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
private:
	using LhsPlain = remove_all_t<Lhs>;
	using RhsPlain = remove_all_t<Lhs>;
	constexpr static Flag LhsFlags = traits<LhsPlain>::Flags;
	constexpr static Flag RhsFlags = traits<RhsPlain>::Flags;
	constexpr static bool SameType = std::is_same_v<typename Lhs::Scalar, typename Rhs::Scalar>;
	constexpr static bool StorageOrdersAgree = (LhsFlags & Flag::RowMajor) == (RhsFlags & Flag::RowMajor);
	// 判断是否要开启LinearAccessBit和PacketAccessBit位
	constexpr static Flag Flags0 = (LhsFlags & RhsFlags) &
			((StorageOrdersAgree ? Flag::LinearAccess : Flag::None) |
				(StorageOrdersAgree && SameType ? Flag::PacketAccess : Flag::None));

public:
	using Scalar = traits<LhsPlain>::Scalar;
	constexpr static int RowsAtCompileTime = traits<LhsPlain>::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = traits<LhsPlain>::ColsAtCompileTime;
	constexpr static Flag Flags = Flags0 | (LhsFlags & Flag::RowMajor);	// 取Lhs的RowMajorBit标志位
	constexpr static int Alignment = traits<LhsPlain>::Alignment < traits<RhsPlain>::Alignment ? 
		traits<LhsPlain>::Alignment : traits<RhsPlain>::Alignment;
};

// 二元运算表达式
template<typename BinaryOp, typename LhsType, typename RhsType>
class CwiseBinaryOp : public MatrixBase<CwiseBinaryOp<BinaryOp, LhsType, RhsType>>
{
public:
	using Functor	= remove_all_t<BinaryOp>;
	using LhsPlain	= remove_all_t<LhsType>;
	using RhsPlain	= remove_all_t<RhsType>;
	using LhsNested = ref_selector<LhsPlain>::type;
	using RhsNested = ref_selector<RhsPlain>::type;

	static_assert(have_same_matrix_size<LhsPlain, RhsPlain>, "You mix matrices of different sizes.");

public:
	CwiseBinaryOp(const LhsPlain& lhs, const RhsPlain& rhs, const Functor& functor = Functor())
			: m_lhs(lhs), m_rhs(rhs), m_functor(functor)
	{}

	CwiseBinaryOp(const CwiseBinaryOp<BinaryOp, LhsType, RhsType>&) = default;

	constexpr int rows() const
	{
		return traits<LhsPlain>::RowsAtCompileTime == Dynamic ? m_rhs.rows() : m_lhs.rows();
	}
	constexpr int cols() const
	{
		return traits<LhsPlain>::ColsAtCompileTime == Dynamic ? m_rhs.cols() : m_lhs.cols();
	}
	const LhsPlain& lhs() const { return m_lhs; }
	const RhsPlain& rhs() const { return m_rhs; }
	const Functor& functor() const { return m_functor; }

private:
	const Functor m_functor;
	LhsNested m_lhs;
	RhsNested m_rhs;
};

}