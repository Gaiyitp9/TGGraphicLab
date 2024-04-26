/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar, int Index, int Stop>
	struct DefaultDot
	{
		static void Run(Scalar& dot, Scalar const* left, Scalar const* right)
		{
			dot += left[Index] * right[Index];
			DefaultDot<Scalar, Index + 1, Stop>::Run(dot, left, right);
		}
	};

	template<typename Scalar, int Stop>
	struct DefaultDot<Scalar, Stop, Stop>
	{
		static void Run(Scalar&, Scalar const*, Scalar const*) {}
	};

	template<typename Matrix>
	struct DotOp
	{
		using Scalar = Traits<Matrix>::Scalar;
		constexpr static int Size = Traits<Matrix>::Size;

		static Scalar Run(const Matrix& left, const Matrix& right)
		{
			Scalar dot = 0;
			DefaultDot<Scalar, 0, Size>::Run(dot, left.m_data(), right.m_data());
			return dot;
		}
	};
}
