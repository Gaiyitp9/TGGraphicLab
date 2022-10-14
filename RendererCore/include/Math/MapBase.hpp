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

template<typename Derived>
class MapBase<Derived, AccessorLevel::ReadOnly> : public MatrixBase<Derived>
{
public:
	using Base = MatrixBase<Derived>;
	using Base::RowsAtCompileTime;
	using Base::ColsAtCompileTime;
	using Base::SizeAtCompileTime;
	using Base::IsVectorAtCompileTime;
	using typename Base::Scalar;
	using typename Base::CoeffReturnType;
	using PointerType = std::conditional_t<is_lvalue<Derived>, Scalar*, const Scalar*>;

	using Base::innerStride;
	using Base::outerStride;
	using Base::rowStride;
	using Base::colStride;

	MapBase(PointerType dataPtr) : m_data(dataPtr), m_rows(RowsAtCompileTime), m_cols(ColsAtCompileTime)
	{
		static_assert(SizeAtCompileTime != Dynamic, "This method is only for fixed size.");
	}

	MapBase(PointerType dataPtr, int rows, int cols) : m_data(dataPtr), m_rows(rows), m_cols(cols)
	{
		assert((rows >= 0 && (RowsAtCompileTime == Dynamic || RowsAtCompileTime == rows))
			&& (cols >= 0 && (ColsAtCompileTime == Dynamic || ColsAtCompileTime == cols)));
	}

	int rows() const noexcept { return m_rows; }
	int cols() const noexcept { return m_cols; }

	const Scalar* data() const { return m_data; }
	CoeffReturnType coeff(int row, int col) const
	{
		return m_data[row * rowStride() + col * colStride()];
	}

	CoeffReturnType coeff(int index) const
	{
		static_assert(NotNone(traits<Derived>::Flags & Flag::LinearAccess) ||
			IsVectorAtCompileTime);
		return m_data[index * innerStride()];
	}

protected:
	PointerType m_data;
	int m_rows;
	int m_cols;
};

template<typename Derived>
class MapBase<Derived, AccessorLevel::Write> : public MapBase<Derived, AccessorLevel::ReadOnly>
{
public:
	using Base = MapBase<Derived, AccessorLevel::ReadOnly>;
	using Base::IsVectorAtCompileTime;
	using typename Base::Scalar;
	using typename Base::PointerType;
	typedef std::conditional_t<
		is_lvalue<Derived>,
		Scalar,
		const Scalar
	> ScalarWithConstIfNotLvalue;

	using Base::innerStride;
	using Base::outerStride;
	using Base::rowStride;
	using Base::colStride;
	using Base::rows;
	using Base::cols;
	using Base::coeff;

	MapBase(PointerType dataPtr) : Base(dataPtr) {}
	MapBase(PointerType dataPtr, int rows, int cols) : Base(dataPtr, rows, cols) {}

	const Scalar* data() const { return this->m_data; }
	ScalarWithConstIfNotLvalue* data() { return this->m_data; }

	ScalarWithConstIfNotLvalue& coeffRef(int row, int col)
	{
		return this->data[row * rowStride() + col * colStride()];
	}

	ScalarWithConstIfNotLvalue& coeffRef(int index)
	{
		static_assert(NotNone(traits<Derived>::Flags & Flag::LinearAccess) ||
			IsVectorAtCompileTime);
		return this->data[index * innerStride()];
	}
};

}