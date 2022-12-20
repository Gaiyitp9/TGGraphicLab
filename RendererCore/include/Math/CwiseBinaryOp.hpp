/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 加法函数
	template<typename Scalar>
	class ScalarSumOp
	{
	public:
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a + b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return padd(a, b); }
	};
	// 减法函数
	template<typename Scalar>
	class ScalarSubOp
	{
	public:
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a - b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return psub(a, b); }
	};
	// 乘法函数
	template<typename Scalar>
	class ScalarProductOp
	{
	public:
		Scalar operator()(const Scalar& a, const Scalar& b) const { return a * b; }

		template<typename Packet>
		Packet PacketOp(const Packet& a, const Packet& b) const { return pmul(a, b); }
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
	template<typename Matrix, typename Functor, int Index, int Stop>
	struct VectorizedBinaryOp
	{
		static void Run(Matrix& dst, const Matrix& left, const Matrix& right, const Functor& functor)
		{
			// 根据矩阵是否对齐来选择函数版本
			pstoret<Matrix::PacketType, Matrix::IsAligned>(
				dst.data() + Index,
				functor.template PacketOp<Matrix::PacketType>(
					ploadt<Matrix::PacketType, Matrix::IsAligned>(left.data() + Index),
					ploadt<Matrix::PacketType, Matrix::IsAligned>(right.data() + Index))
			);
			constexpr static int NextIndex = Index + unpacket_traits<Matrix::PacketType>::Size;
			VectorizedBinaryOp<Matrix, Functor, NextIndex, Stop>::Run(dst, left, right, functor);
		}
	};

	template<typename Matrix, typename Functor, int Stop>
	struct VectorizedBinaryOp<Matrix, Functor, Stop, Stop>
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
			if constexpr (Support_SIMD)
			{
				constexpr static int vectorizableSize = (Matrix::SizeAtCompileTime / unpacket_traits<Matrix::PacketType>::Size)
					* unpacket_traits<Matrix::PacketType>::Size;
				VectorizedBinaryOp<Matrix, Functor, 0, vectorizableSize>::Run(dst, left, right, functor);
				DefaultBinaryOp<Matrix, Functor, vectorizableSize, Matrix::SizeAtCompileTime>::Run(dst, left, right, functor);
			}
			else
			{
				DefaultBinaryOp<Matrix, Functor, 0, Matrix::SizeAtCompileTime>::Run(dst, left, right, functor);
			}
			return dst;
		}
	};
}