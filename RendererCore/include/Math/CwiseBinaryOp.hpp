/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	// 加法函数
	template<typename Scalar>
	struct ScalarSumOp
	{
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a + b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return padd(a, b); }
	};
	// 减法函数
	template<typename Scalar>
	struct ScalarSubOp
	{
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a - b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return psub(a, b); }
	};
	// 乘法函数
	template<typename Scalar>
	struct ScalarProductOp
	{
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a * b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return pmul(a, b); }
	};
	// 除法函数
	template<typename Scalar>
	struct ScalarDivideOp
	{
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a / b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return pdiv(a, b); }
	};

	// 直接展开计算
	template<typename Matrix, typename Functor, int Index, int Stop>
	struct DefaultBinaryOp
	{
		static void Run(Matrix& dst, const Matrix& left, const Matrix& right, const Functor& functor)
		{
			dst[Index] = functor(left[Index], right[Index]);
			DefaultBinaryOp<Matrix, Functor, Index + 1, Stop>::Run(dst, left, right, functor);
		}
	};
	template<typename Matrix, typename Functor, int Stop>
	struct DefaultBinaryOp<Matrix, Functor, Stop, Stop>
	{
		static void Run(Matrix&, const Matrix&, const Matrix&, const Functor&) {}
	};

	// 向量化计算
	template<typename Matrix, typename Functor, typename PacketType, bool IsAligned, int Index, int Stop>
	struct VectorizedBinaryOp
	{
		static void Run(Matrix& dst, const Matrix& left, const Matrix& right, const Functor& functor)
		{
			// 根据矩阵是否对齐来选择函数版本
			pstoret<PacketType, IsAligned>(
				dst.data() + Index,
				functor.template PacketOp<PacketType>(
					ploadt<PacketType, IsAligned>(left.data() + Index),
					ploadt<PacketType, IsAligned>(right.data() + Index))
			);
			constexpr static int NextIndex = Index + unpacket_traits<PacketType>::Size;
			VectorizedBinaryOp<Matrix, Functor, PacketType, IsAligned, NextIndex, Stop>::Run(dst, left, right, functor);
		}
	};

	template<typename Matrix, typename Functor, typename PacketType, bool IsAligned, int Stop>
	struct VectorizedBinaryOp<Matrix, Functor, PacketType, IsAligned, Stop, Stop>
	{
		static void Run(Matrix&, const Matrix&, const Matrix&, const Functor&) {}
	};

	template<typename Matrix, typename Functor>
	struct CwiseBinaryOp
	{
		static Matrix Run(const Matrix& left, const Matrix& right)
		{
			Matrix dst;
			Functor functor;
			if constexpr (SUPPORT_SIMD)
			{
				using Scalar = traits<Matrix>::Scalar;
				constexpr static int SizeAtCompileTime = traits<Matrix>::SizeAtCompileTime;
				using PacketType = best_packet<Scalar, SizeAtCompileTime>;
				constexpr static int Alignment = traits<Matrix>::IsDynamic ? DEFAULT_ALIGN_BYTES 
					: default_alignment<Scalar, SizeAtCompileTime>;
				constexpr static bool IsAligned = Alignment >= unpacket_traits<PacketType>::Alignment;
				constexpr static int VectorizableSize = (SizeAtCompileTime / unpacket_traits<PacketType>::Size)
					* unpacket_traits<PacketType>::Size;

				VectorizedBinaryOp<Matrix, Functor, PacketType, IsAligned, 0, VectorizableSize>::Run(dst, left, right, functor);
				DefaultBinaryOp<Matrix, Functor, VectorizableSize, SizeAtCompileTime>::Run(dst, left, right, functor);
			}
			else
			{
				DefaultBinaryOp<Matrix, Functor, 0, traits<Matrix>::SizeAtCompileTime>::Run(dst, left, right, functor);
			}
			return dst;
		}
	};
}