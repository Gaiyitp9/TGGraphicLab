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
class CoeffsBase<Derived, AccessorLevel::ReadOnly> : public Base<Derived>
{
public:
	using Scalar = traits<Derived>::Scalar;
	using CoeffReturnType = std::conditional_t<NotNone(traits<Derived>::Flags & Flag::Lvalue), 
												const Scalar&, const Scalar>;
	using Base = Base<Derived>;
	using Base::rows;
	using Base::cols;
	using Base::size;
	using Base::derived;

	int rowIndexByOuterInner(int outer, int inner) const
	{
		return Derived::RowsAtCompileTime == 1 ? 0 :
			Derived::ColsAtCompileTime == 1 ? inner :
			NotNone(Derived::Flags & Flag::RowMajor) ? outer : inner;
	}

	int colIndexByOuterInner(int outer, int inner) const
	{
		return Derived::ColsAtCompileTime == 1 ? 0 :
			Derived::RowsAtCompileTime == 1 ? inner :
			NotNone(Derived::Flags & Flag::RowMajor) ? inner : outer;
	}

	CoeffReturnType coeff(int row, int col) const
	{
		return Evaluator<Derived>(derived()).Coeff(row, col);
	}

	CoeffReturnType coeffByOuterInner(int outer, int inner) const
	{
		return coeff(rowIndexByOuterInner(outer, inner),
					 colIndexByOuterInner(outer, inner));
	}

	CoeffReturnType operator()(int row, int col) const
	{
		return coeff(row, col);
	}

	CoeffReturnType coeff(int index) const
	{
		static_assert(NotNone(Evaluator<Derived>::Flags & Flag::LinearAccess));
		return Evaluator<Derived>(derived()).Coeff(index);
	}

	CoeffReturnType operator[](int index) const
	{
		static_assert(Derived::IsVectorAtCompileTime);
		return coeff(index);
	}

	CoeffReturnType operator()(int index) const
	{
		static_assert(Derived::IsVectorAtCompileTime);
		return coeff(index);
	}

	CoeffReturnType x() const
	{
		return (*this)[0];
	}

	CoeffReturnType y() const
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 2);
		return (*this)[1];
	}

	CoeffReturnType z() const
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 3);
		return (*this)[2];
	}

	CoeffReturnType w() const
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 4);
		return (*this)[3];
	}
};

template<typename Derived>
class CoeffsBase<Derived, AccessorLevel::Write> : public CoeffsBase<Derived, AccessorLevel::ReadOnly>
{
public:
	using Base = CoeffsBase<Derived, AccessorLevel::ReadOnly>;
	using Scalar = traits<Derived>::Scalar;

	using Base::coeff;
	using Base::rows;
	using Base::cols;
	using Base::size;
	using Base::derived;
	using Base::rowIndexByOuterInner;
	using Base::colIndexByOuterInner;
	using Base::operator[];
	using Base::operator();

	Scalar& coeffRef(int row, int col)
	{
		return Evaluator<Derived>(derived()).coeffRef(row, col);
	}

	Scalar& coeffRefByOuterInner(int outer, int inner)
	{
		return coeffRef(rowIndexByOuterInner(outer, inner),
						colIndexByOuterInner(outer, inner));
	}

	Scalar& operator()(int row, int col)
	{
		return coeffRef(row, col);
	}

	Scalar& coeffRef(int index)
	{
		static_assert(NotNone(Evaluator<Derived>::Flags & Flag::LinearAccess));
		return Evaluator<Derived>(derived()).coeffRef(index);
	}

	Scalar& operator[](int index)
	{
		static_assert(Derived::IsVectorAtCompileTime);
		return coeffRef(index);
	}

	Scalar& operator()(int index)
	{
		return coeffRef(index);
	}

	Scalar& x()
	{
		return (*this)[0];
	}

	Scalar& y()
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 2);
		return (*this)[1];
	}

	Scalar& z()
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 3);
		return (*this)[2];
	}

	Scalar& w()
	{
		static_assert(Derived::SizeAtCompileTime == -1 || Derived::SizeAtCompileTime >= 4);
		return (*this)[3];
	}
};

template<typename Derived>
class CoeffsBase<Derived, AccessorLevel::Direct> : public CoeffsBase<Derived, AccessorLevel::ReadOnly>
{
public:
	using Base = CoeffsBase<Derived, AccessorLevel::ReadOnly>;
	using Scalar = traits<Derived>::Scalar;

	using Base::rows;
	using Base::cols;
	using Base::size;
	using Base::derived;

	constexpr int innerStride() const
	{
		return derived().innerStride();
	}

	constexpr int outerStride() const
	{
		return derived().outerStride();
	}

	constexpr int rowStride() const
	{
		return Derived::IsRowMajor ? outerStride() : innerStride();
	}

	constexpr int colStride() const
	{
		return Derived::IsRowMajor ? innerStride() : outerStride();
	}
};

template<typename Derived>
class CoeffsBase<Derived, AccessorLevel::DirectWrite> : public CoeffsBase<Derived, AccessorLevel::Write>
{
public:
	using Base = CoeffsBase<Derived, AccessorLevel::Write>;
	using Scalar = traits<Derived>::Scalar;

	using Base::rows;
	using Base::cols;
	using Base::size;
	using Base::derived;

	constexpr int innerStride() const
	{
		return derived().innerStride();
	}

	constexpr int outerStride() const
	{
		return derived().outerStride();
	}

	constexpr int rowStride() const
	{
		return Derived::IsRowMajor ? outerStride() : innerStride();
	}

	constexpr int colStride() const
	{
		return Derived::IsRowMajor ? innerStride() : outerStride();
	}
};

}