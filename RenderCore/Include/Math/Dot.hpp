/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar, typename PacketType, bool IsAligned, int Index, int Stop>
	struct VectorizedDot
	{
		static void Run(Scalar& dot, Scalar const* left, Scalar const* right)
		{
			Scalar temp[unpacket_traits<PacketType>::Size];
			pstoreu(temp,
				pmul(ploadt<PacketType, IsAligned>(left + Index),
					ploadt<PacketType, IsAligned>(right + Index))
			);
			for (int i = 0; i < unpacket_traits<PacketType>::Size; ++i)
				dot += temp[i];

			constexpr static int NextIndex = Index + unpacket_traits<PacketType>::Size;
			VectorizedDot<Scalar, PacketType, IsAligned, NextIndex, Stop>::Run(dot, left, right);
		}
	};

	template<typename Scalar, typename PacketType, bool IsAligned, int Stop>
	struct VectorizedDot<Scalar, PacketType, IsAligned, Stop, Stop>
	{
		static void Run(Scalar&, Scalar const*, Scalar const*) {}
	};

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
		using Scalar = traits<Matrix>::Scalar;
		constexpr static int SizeAtCompileTime = traits<Matrix>::SizeAtCompileTime;

		static Scalar Run(const Matrix& left, const Matrix& right)
		{
			Scalar dot = 0;
			if constexpr(SUPPORT_SIMD)
			{
				using PacketType = best_packet<Scalar, SizeAtCompileTime>;
				constexpr static int Alignment = traits<Matrix>::IsDynamic ? DEFAULT_ALIGN_BYTES
					: default_alignment<Scalar, SizeAtCompileTime>;
				constexpr static bool IsAligned = Alignment >= unpacket_traits<PacketType>::Alignment;
				constexpr static int VectorizableSize = (SizeAtCompileTime / unpacket_traits<PacketType>::Size)
					* unpacket_traits<PacketType>::Size;
				VectorizedDot<Scalar, PacketType, IsAligned, 0, VectorizableSize>::Run(dot, left.m_data(), right.m_data());
				DefaultDot<Scalar, VectorizableSize, SizeAtCompileTime>::Run(dot, left.m_data(), right.m_data());
			}
			else
			{
				DefaultDot<Scalar, 0, SizeAtCompileTime>::Run(dot, left.m_data(), right.m_data());
			}
			return dot;
		}
	};
}