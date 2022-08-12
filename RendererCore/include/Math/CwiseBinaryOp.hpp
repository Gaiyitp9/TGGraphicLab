/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 二元运算表达式
	template<typename BinaryOp, typename LhsType, typename RhsType>
	class CwiseBinaryOp : public MatrixBase<CwiseBinaryOp<BinaryOp, LhsType, RhsType>>
	{
		using Functor	= remove_all_t<BinaryOp>;
		using Lhs		= remove_all_t<LhsType>;
		using Rhs		= remove_all_t<RhsType>;
		using LhsNested = ref_selector<Lhs>::type;
		using RhsNested = ref_selector<Rhs>::type;

	public:
		CwiseBinaryOp(const Lhs& lhs, const Rhs& rhs, const Functor& functor = Functor())
			: m_lhs(lhs), m_rhs(rhs), m_functor(functor)
		{}

	private:
		const Functor m_functor;
		LhsNested m_lhs;
		RhsNested m_rhs;
	};
}