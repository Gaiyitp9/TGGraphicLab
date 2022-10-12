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

template<typename XprType, int BlockRows, int BlockCols>
struct traits<Block<XprType, BlockRows, BlockCols>> : traits<XprType>
{
	using Scalar = traits<XprType>::Scalar;
	constexpr static int RowsAtCompileTime = BlockRows;
	constexpr static int ColsAtCompileTime = BlockCols;
	constexpr static bool XprIsRowMajor = not_none(traits<XprType>::Flags & Flag::RowMajor);
	constexpr static bool IsRowMajor = (BlockRows == 1 && BlockCols != 1) ? true
										: (BlockCols == 1 && BlockRows != 1) ? false 
										: XprIsRowMajor;
	constexpr static bool HasSameStorageOrderAsXprType = XprIsRowMajor == IsRowMajor;
	constexpr static int InnerStrideAtCompileTime = HasSameStorageOrderAsXprType  
													? inner_stride_at_compile_time<XprType>::value
													: outer_stride_at_compile_time<XprType>::value;
	constexpr static int OuterStrideAtCompileTime = HasSameStorageOrderAsXprType  
													? outer_stride_at_compile_time<XprType>::value
													: inner_stride_at_compile_time<XprType>::value;
	constexpr static Flag FlagLvalue = is_lvalue<XprType> ? Flag::Lvalue : Flag::None;
	constexpr static Flag FlagRowMajor = IsRowMajor ? Flag::RowMajor : Flag::None;
	constexpr static Flag Flags = FlagLvalue | FlagRowMajor;
	constexpr static int Alignment = 0;
};

template<typename XprType, int BlockRows, int BlockCols, bool HasDirectAccess>
class Block : public MatrixBase<Block<XprType, BlockRows, BlockCols, HasDirectAccess>>
{
public:
	using Base = MatrixBase<Block>;
	using typename Base::Scalar;
	using typename Base::CoeffReturnType;
	using Base::RowsAtCompileTime;
	using Base::ColsAtCompileTime;
	using XprTypePlain = remove_all_t<XprType>;
	using XprTypeNested = ref_selector<XprTypePlain>::non_const_type;

	Block(XprType& xpr, int index)
		: m_xpr(xpr), m_startRow((BlockRows == 1) && (BlockCols == XprType::ColsAtCompileTime) ? index : 0),
		m_startCol((BlockCols == 1) && (BlockRows == XprType::RowsAtCompileTime) ? index : 0),
		m_blockRows(BlockRows == 1 ? 1 : xpr.rows()),
		m_blockCols(BlockCols == 1 ? 1 : xpr.cols())
	{}

	Block(XprType& xpr, int startRow, int startCol)
		: m_xpr(xpr), m_startRow(startRow), m_startCol(startCol)
	{
		static_assert(RowsAtCompileTime != Dynamic && ColsAtCompileTime != Dynamic, "This method is only for fixed size.");
		assert(startRow >= 0 && BlockRows >= 0 && startRow + BlockRows <= xpr.rows()
			&& startCol >= 0 && BlockCols >= 0 && startCol + BlockCols <= xpr.cols());
	}

	Block(XprType& xpr, int startRow, int startCol, int blockRows, int blockCols)
		: m_xpr(xpr), m_startRow(startRow), m_startCol(startCol), m_blockRows(blockRows), m_blockCols(blockCols)
	{
		assert((RowsAtCompileTime == Dynamic || RowsAtCompileTime == blockRows)
			&& (ColsAtCompileTime == Dynamic || ColsAtCompileTime == blockCols));
		assert(startRow >= 0 && blockRows >= 0 && startRow <= xpr.rows() - blockRows
			&& startCol >= 0 && blockCols >= 0 && startCol <= xpr.cols() - blockCols);
	}

	int rows() const { return m_blockRows; }
	int cols() const { return m_blockCols; }

	CoeffReturnType coeff(int row, int col) const
	{
		return m_xpr.coeff(m_startRow + row, m_startCol + col);
	}

	Scalar& coeffRef(int row, int col)
	{
		static_assert(is_lvalue<XprType>, "The expression is not a lvalue. It is read only.");
		return m_xpr.coeffRef(m_startRow + row, m_startCol + col);
	}

	const XprTypePlain& nestedExpression() const { return m_xpr; }
	XprTypePlain& nestedExpression() { return m_xpr; }

private:
	XprTypeNested m_xpr;
	int m_startRow;
	int m_startCol;
	int m_blockRows;
	int m_blockCols;
};

template<typename XprType, int BlockRows, int BlockCols>
class Block<XprType, BlockRows, BlockCols, true>
	: public MapBase<Block<XprType, BlockRows, BlockCols, true>>
{
	using Base = MapBase<Block<XprType, BlockRows, BlockCols, true>>;
	using XprTypePlain = remove_all_t<XprType>;
	using XprTypeNested = ref_selector<XprTypePlain>::non_const_type;

public:
	// 构建行或者列
	Block(XprType& xpr, int index) 
		: Base(xpr.data() + index * ((BlockRows == 1 && !XprType::IsRowMajor) ||
								(BlockCols == 1 && XprType::IsRowMajor) 
								? xpr.innerStride() : xpr.outerStride()),
			BlockRows == 1 ? 1 : xpr.rows(),
			BlockCols == 1 ? 1 : xpr.cols()),
		m_xpr(xpr),
		m_startRow(BlockRows == 1 ? index : 0),
		m_startCol(BlockCols == 1 ? index : 0)
	{
		static_assert((BlockRows == 1 && BlockCols == XprType::ColsAtCompileTime) ||
			(BlockRows == XprType::RowsAtCompileTime && BlockCols == 1), 
			"The Block rows should be same as XprType for column vector"
			"or Block cols should be same as XprType for row vector.");
	}

	Block(XprType& xpr, int startRow, int startCol)
		: Base(xpr.data() + xpr.innerStride() * (XprType::IsRowMajor ? startCol : startRow) + 
		xpr.outerStride() * (XprType::IsRowMajor ? startRow : startRow)),
		m_xpr(xpr), m_startRow(startRow), m_startCol(startCol)
	{
		assert(startRow >= 0 && BlockRows >= 0 && startRow + BlockRows <= xpr.rows()
			&& startCol >= 0 && BlockCols >= 0 && startCol + BlockCols <= xpr.cols());
	}

	Block(XprType& xpr, int startRow, int startCol, int blockRows, int blockCols)
		: Base(xpr.data() + xpr.innerStride() * (XprType::IsRowMajor ? startCol : startRow) +
			xpr.outerStride() * (XprType::IsRowMajor ? startRow : startRow), blockRows, blockCols),
		m_xpr(xpr), m_startRow(startRow), m_startCol(startCol)
	{

	}

	int innerStride()
	{
		if constexpr (traits<Block>::HasSameStorageOrderAsXprType)
			return m_xpr.innerStride();
		else
			return m_xpr.outerStride();
	}

	int outerStride()
	{
		if constexpr (traits<Block>::HasSameStorageOrderAsXprType)
			return m_xpr.outerStride();
		else
			return m_xpr.innerStride();
	}

	const XprTypePlain& nestedExpression() const { return m_xpr; }
	XprTypePlain& nestedExpression() { return m_xpr; }

private:
	XprTypeNested m_xpr;
	int m_startRow;
	int m_startCol;
};

}