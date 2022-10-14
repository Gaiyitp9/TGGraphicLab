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

// 一元运算求值器
template<typename T, 
		 typename Scalar = typename T::Scalar>
class UnaryEvaluator;
// 二元运算求值器
template<typename T, 
		 typename LhsScalar = typename traits<typename T::LhsPlain>::Scalar,
		 typename RhsScalar = typename traits<typename T::RhsPlain>::Scalar>
class BinaryEvaluator;

template<typename T>
inline constexpr bool evaluator_assume_aliasing = false;

template<typename T>
class Evaluator : public UnaryEvaluator<T>
{
public:
	using Base = UnaryEvaluator<T>;
	explicit Evaluator(const T& xpr) : Base(xpr) {}
};

// 矩阵
template<typename Scalar_, int Rows, int Cols, StorageOption Options>
class Evaluator<Matrix<Scalar_, Rows, Cols, Options>>
{
public:
	using XprType = Matrix<Scalar_, Rows, Cols, Options>;
	using Scalar = XprType::Scalar;
	using CoeffReturnType = XprType::CoeffReturnType;

	constexpr static bool IsRowMajor = XprType::IsRowMajor;
	constexpr static bool IsVectorAtCompileTime = XprType::IsVectorAtCompileTime;
	constexpr static int RowsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static Flag Flags = traits<XprType>::Flags;
	constexpr static int Alignment = traits<XprType>::Alignment;
	constexpr static int OuterStrideAtCompileTime = XprType::OuterStrideAtCompileTime;

	Evaluator() : m_data(nullptr), m_outerStride(OuterStrideAtCompileTime) {}
	explicit Evaluator(const XprType& m) : m_data(m.data()), m_outerStride(m.outerStride()) {}

	CoeffReturnType coeff(int row, int col) const
	{
		if constexpr (IsRowMajor)
			return m_data[row * m_outerStride + col];
		else
			return m_data[row + col * m_outerStride];
	}

	CoeffReturnType coeff(int index) const
	{
		return m_data[index];
	}

	Scalar& coeffRef(int row, int col)
	{
		if constexpr (IsRowMajor)
			return const_cast<Scalar*>(m_data)[row * m_outerStride + col];
		else
			return const_cast<Scalar*>(m_data)[row + col * m_outerStride];
	}

	Scalar& coeffRef(int index)
	{
		return const_cast<Scalar*>(m_data)[index];
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		if constexpr (IsRowMajor)
			return ploadt<PacketType, LoadMode>(m_data + row * m_outerStride + col);
		else
			return ploadt<PacketType, LoadMode>(m_data + row + col * m_outerStride);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return ploadt<PacketType, LoadMode>(m_data + index);
	}

private:
	const Scalar* m_data;
	int m_outerStride;
};

// 转置
template<typename ArgType>
class UnaryEvaluator<Transpose<ArgType>>
{
public:
	using XprType = Transpose<ArgType>;
	using Scalar = XprType::Scalar;
	using CoeffReturnType = XprType::CoeffReturnType;
	constexpr static int RowsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static Flag Flags = traits<XprType>::Flags;
	constexpr static int Alignment = traits<XprType>::Alignment;

	explicit UnaryEvaluator(const XprType& t) : m_argImpl(t.nestedExpression()) {}

	CoeffReturnType coeff(int row, int col) const
	{
		return m_argImpl.coeff(col, row);
	}

	CoeffReturnType coeff(int index) const
	{
		return m_argImpl.coeff(index);
	}

	Scalar& coeffRef(int row, int col)
	{
		return m_argImpl.coeffRef(col, row);
	}

	Scalar& coeffRef(int index)
	{
		return m_argImpl.coeffRef(index);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		return m_argImpl.template packet<LoadMode, PacketType>(col, row);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return m_argImpl.template packet<LoadMode, PacketType>(index);
	}

private:
	Evaluator<ArgType> m_argImpl;
};

// 二元运算
template<typename BinaryOp, typename Lhs, typename Rhs>
class Evaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
	: public BinaryEvaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
	using Base = BinaryEvaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>;
	using typename Base::XprType;
public:
	explicit Evaluator(const XprType& xpr) : Base(xpr) {}
};

template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryEvaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
public:
	using XprType = CwiseBinaryOp<BinaryOp, Lhs, Rhs>;
	using CoeffReturnType = XprType::CoeffReturnType;

	constexpr static int Alignment = traits<XprType>::Alignment;
	constexpr static Flag Flags = traits<XprType>::Flags;

	explicit BinaryEvaluator(const XprType& xpr) : m_op(xpr.functor()), m_lhsImpl(xpr.lhs()), 
		m_rhsImpl(xpr.rhs()) {}

	CoeffReturnType coeff(int row, int col) const
	{
		return m_op(m_lhsImpl.coeff(row, col), m_rhsImpl.coeff(row, col));
	}

	CoeffReturnType coeff(int index) const
	{
		return m_op(m_lhsImpl.coeff(index), m_rhsImpl.coeff(index));
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		return m_op.PacketOp(m_lhsImpl.template packet<LoadMode, PacketType>(row, col),
			m_rhsImpl.template packet<LoadMode, PacketType>(row, col));
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return m_op.PacketOp(m_lhsImpl.template packet<LoadMode, PacketType>(index),
			m_rhsImpl.template packet<LoadMode, PacketType>(index));
	}

private:
	BinaryOp m_op;
	Evaluator<Lhs> m_lhsImpl;
	Evaluator<Rhs> m_rhsImpl;
};

// 块
template<typename ArgType, int BlockRows, int BlockCols, bool HasDirectAccess = has_direct_access<ArgType>>
class BlockEvaluator;

template<typename ArgType, int BlockRows, int BlockCols>
class Evaluator<Block<ArgType, BlockRows, BlockCols>> : public BlockEvaluator<ArgType, BlockRows, BlockCols>
{
public:
	using XprType = Block<ArgType, BlockRows, BlockCols>;
	using Scalar = XprType::Scalar;
	using PacketScalar = best_packet<Scalar, XprType::SizeAtCompileTime>;

	constexpr static int RowsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static int Alignment = traits<XprType>::Alignment;
	constexpr static Flag Flags = traits<XprType>::Flags;
};

template<typename ArgType, int BlockRows, int BlockCols>
class BlockEvaluator<ArgType, BlockRows, BlockCols, false> 
	: public UnaryEvaluator<Block<ArgType, BlockRows, BlockCols>>
{
	using Base = UnaryEvaluator<Block<ArgType, BlockRows, BlockCols>>;
	using XprType = Block<ArgType, BlockRows, BlockCols>;
public:
	explicit BlockEvaluator(const XprType& xpr) : Base(xpr) {}
};

template<typename ArgType, int BlockRows, int BlockCols>
class UnaryEvaluator<Block<ArgType, BlockRows, BlockCols>>
{
	using XprType = Block<ArgType, BlockRows, BlockCols>;
	using Scalar = XprType::Scalar;
	using CoeffReturnType = XprType::CoeffReturnType;

public:
	UnaryEvaluator(const XprType& block) 
		: m_argImpl(block), m_startRow(block.startRow()), m_startCol(block.startCol())
	{}

	CoeffReturnType coeff(int row, int col) const
	{
		return m_argImpl.coeff(m_startRow + row, m_startCol + col);
	}

	CoeffReturnType coeff(int index) const
	{
		if constexpr (XprType::RowsAtCompileTime == 1)
			return coeff(m_startRow, m_startCol + index);
		else
			return coeff(m_startRow + index, m_startCol);
	}

	Scalar& coeffRef(int row, int col)
	{
		return m_argImpl.coeffRef(m_startRow + row, m_startCol + col);
	}

	Scalar& coeffRef(int index)
	{
		if constexpr (XprType::RowsAtCompileTime == 1)
			return coeff(m_startRow, m_startCol + index);
		else
			return coeff(m_startRow + index, m_startCol);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		return m_argImpl.packet<LoadMode, PacketType>(m_startRow + row, m_startCol + col);
	}

private:
	Evaluator<XprType> m_argImpl;
	int m_startRow;
	int m_startCol;
};

}