/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once

namespace LCH::Math
{

template<typename Derived>
class CoeffsBase<Derived, AccessorLevel::ReadOnly> : public Base<Derived>
{
public:
	using Scalar = traits<Derived>::Scalar;
	using CoeffReturnType = std::conditional_t<not_none(traits<Derived>::Flags & Flag::Lvalue), 
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
			not_none(Derived::Flags & Flag::RowMajor) ? outer : inner;
	}

	int colIndexByOuterInner(int outer, int inner) const
	{
		return Derived::ColsAtCompileTime == 1 ? 0 :
			Derived::RowsAtCompileTime == 1 ? inner :
			not_none(Derived::Flags & Flag::RowMajor) ? inner : outer;
	}

	CoeffReturnType coeff(int row, int col) const
	{
		return evaluator<Derived>(derived()).Coeff(row, col);
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
		static_assert(not_none(evaluator<Derived>::Flags & Flag::LinearAccess));
		return evaluator<Derived>(derived()).Coeff(index);
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
		return evaluator<Derived>(derived()).coeffRef(row, col);
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
		static_assert(not_none(evaluator<Derived>::Flags & Flag::LinearAccess));
		return evaluator<Derived>(derived()).coeffRef(index);
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

template<typename Derived, bool HasDirectAccess = has_direct_access<Derived>::value>
struct inner_stride_at_compile_time
{
	constexpr static int value = traits<Derived>::InnerStrideAtCompileTime;
};

template<typename Derived>
struct inner_stride_at_compile_time<Derived, false>
{
	constexpr static int value = 0;
};

template<typename Derived, bool HasDirectAccess = has_direct_access<Derived>::value>
struct outer_stride_at_compile_time
{
	constexpr static int value = traits<Derived>::OuterStrideAtCompileTime;
};

template<typename Derived>
struct outer_stride_at_compile_time<Derived, false>
{
	constexpr static int value = 0;
};

}