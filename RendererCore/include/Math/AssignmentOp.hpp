/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 直接展开赋值
	template<typename Matrix, int Index, int Stop>
	struct DefaultAssign
	{
		static void Run(Matrix& dst, const Matrix& src)
		{
			dst[Index] = src[Index];
			DefaultAssign<Matrix, Index + 1, Stop>::Run(dst, src);
		}
	};
	template<typename Matrix, int Stop>
	struct DefaultAssign<Matrix, Stop, Stop>
	{
		static void Run(Matrix&, const Matrix&){}
	};

	// 向量化赋值
	template<typename Matrix, int Index, int Stop>
	struct VectorizedAssign
	{
		static void Run(Matrix& dst, const Matrix& src)
		{
			// 根据矩阵是否对齐来选择函数版本
			pstoret<Matrix::PacketType, Matrix::IsAligned>(dst.data() + Index, 
				ploadt< Matrix::PacketType, Matrix::IsAligned>(src.data() + Index));
			constexpr static int NextIndex = Index + unpacket_traits<Matrix::PacketType>::Size;
			VectorizedAssign<Matrix, NextIndex, Stop>::Run(dst, src);
		}
	};

	template<typename Matrix, int Stop>
	struct VectorizedAssign<Matrix, Stop, Stop>
	{
		static void Run(Matrix&, const Matrix&){}
	};

	template<typename Matrix>
	struct AssignmentOp
	{
		static void Run(Matrix& dst, const Matrix& src)
		{
			if constexpr (Support_SIMD)
			{
				constexpr static int vectorizableSize = (Matrix::SizeAtCompileTime / unpacket_traits<Matrix::PacketType>::Size) 
					* unpacket_traits<Matrix::PacketType>::Size;
				VectorizedAssign<Matrix, 0, vectorizableSize>::Run(dst, src);
				DefaultAssign<Matrix, vectorizableSize, Matrix::SizeAtCompileTime>::Run(dst, src);
			}
			else
			{
				DefaultAssign<Matrix, 0, Matrix::SizeAtCompileTime>::Run(dst, src);
			}
		}
	};
}