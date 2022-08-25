/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Derived>
	class CoeffsBase<Derived, ReadOnlyAccessors> : public Base<Derived>
	{
		using Scalar = traits<Derived>::Scalar;
		using PacketScalar = traits<Derived>::PacketScalar;

		using CoeffReturnType = std::conditional_t<bool(traits<Derived>::Flags&LvalueBit), 
													const Scalar&, const Scalar>;
		using PacketReturnType = add_const_on_value_type_t<PacketScalar>;

		using Base = Base<Derived>;
		using Base::rows;
		using Base::cols;
		using Base::size;
		using Base::derived;

		int RowIndexByOuterInner(int outer, int inner) const
		{
			return Derived::RowsAtCompileTime == 1 ? 0 :
				Derived::ColsAtCompileTime == 1 ? inner :
				Derived::Flags & RowMajorBit ? outer : inner;
		}

		int ColIndexByOuterInner(int outer, int inner) const
		{
			return Derived::ColsAtCompileTime == 1 ? 0 :
				Derived::RowsAtCompileTime == 1 ? inner :
				Derived::Flags & RowMajorBit ? inner : outer;
		}

		CoeffReturnType Coeff(int row, int col) const
		{
			return evaluator<Derived>(derived()).Coeff(row, col);
		}

		CoeffReturnType CoeffByOuterInner(int outer, int inner) const
		{
			return Coeff(RowIndexByOuterInner(outer, inner),
						 ColIndexByOuterInner(outer, inner));
		}

		CoeffReturnType operator()(int row, int col) const
		{
			return Coeff(row, col);
		}

		CoeffReturnType Coeff(int index) const
		{
			static_assert(evaluator<Derived>::Flags & LinearAccessBit);
			return evaluator<Derived>(derived()).Coeff(index);
		}

		CoeffReturnType operator[](int index) const
		{
			static_assert(Derived::IsVectorAtCompileTime);
			return Coeff(index);
		}

		CoeffReturnType operator()(int index) const
		{
			return Coeff(index);
		}

		CoeffReturnType x() const
		{
			return (*this)[0];
		}

		CoeffReturnType y() const
		{
			return (*this)[1];
		}

		CoeffReturnType z() const
		{
			return (*this)[2];
		}

		CoeffReturnType w() const
		{
			return (*this)[3];
		}
	};
}