/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	// 两个矩阵是否可以相乘
	template<typename MatrixL, typename MatrixR>
	concept MatricesMultiplable = traits<MatrixL>::ColsAtCompileTime == traits<MatrixR>::RowsAtCompileTime
		&& std::is_same_v<typename traits<MatrixL>::Scalar, typename traits<MatrixR>::Scalar>
		&& traits<MatrixL>::IsRowMajor == traits<MatrixR>::IsRowMajor;

	template<typename MatrixL, typename MatrixR>
		requires MatricesMultiplable<MatrixL, MatrixR>
	struct MatricesMultipleTraits
	{
	private:
		constexpr static StorageOption Option = traits<MatrixL>::IsRowMajor ? StorageOption::RowMajor
			: StorageOption::ColMajor;

	public:
		using ReturnType = Matrix<
			typename traits<MatrixL>::Scalar,
			traits<MatrixL>::RowsAtCompileTime,
			traits<MatrixR>::ColsAtCompileTime,
			Option
		>;
		constexpr static int Stride = traits<ReturnType>::IsRowMajor
			? traits<ReturnType>::ColsAtCompileTime
			: traits<ReturnType>::RowsAtCompileTime;
		using Scalar = traits<ReturnType>::Scalar;
		constexpr static int SizeAtCompileTime = traits<MatrixL>::ColsAtCompileTime;
		using PacketType = best_packet<Scalar, SizeAtCompileTime>;
		constexpr static int Alignment = traits<ReturnType>::IsDynamic ? DEFAULT_ALIGN_BYTES
			: (traits<ReturnType>::IsRowMajor 
				? default_alignment<Scalar, traits<MatrixL>::SizeAtCompileTime>
				: default_alignment<Scalar, traits<MatrixR>::SizeAtCompileTime>);
		constexpr static bool IsAligned = Alignment >= unpacket_traits<PacketType>::Alignment;
		constexpr static int VectorizableSize = (SizeAtCompileTime / unpacket_traits<PacketType>::Size)
			* unpacket_traits<PacketType>::Size;
	};

	template<typename MatrixL, typename MatrixR, int Index, int Stop>
	struct DefaultMultiple
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;
		using Scalar = traits<MatrixType>::Scalar;
		constexpr static int Row = Index / MatricesMultipleTraits<MatrixL, MatrixR>::Stride;
		constexpr static int Col = Index % MatricesMultipleTraits<MatrixL, MatrixR>::Stride;

		static void Run(MatrixType& dst, const MatrixL& left, const MatrixR& right)
		{
			Scalar sum = 0;
			for (int i = 0; i < traits<MatrixL>::ColsAtCompileTime; ++i)
				sum += left(Row, i) * right(i, Col);
			dst(Row, Col) = sum;

			DefaultMultiple<MatrixL, MatrixR, Index + 1, Stop>::Run(dst, left, right);
		}
	};

	template<typename MatrixL, typename MatrixR, int Stop>
	struct DefaultMultiple<MatrixL, MatrixR, Stop, Stop> 
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;
		static void Run(MatrixType&, const MatrixL&, const MatrixR&) {}
	};

	template<typename MatrixL, typename MatrixR, int Index, int Stop>
	struct VectorizedMultiple
	{
		using Traits = MatricesMultipleTraits<MatrixL, MatrixR>;
		using MatrixType = Traits::ReturnType;
		using Scalar = Traits::Scalar;
		using PacketType = Traits::PacketType;
		constexpr static int Row = Index / Traits::Stride;
		constexpr static int Col = Index % Traits::Stride;

		static void Run(MatrixType& dst, const MatrixL& left, const MatrixR& right)
		{
			if constexpr(traits<MatrixType>::IsRowMajor)
			{
				Scalar rightCol[Traits::SizeAtCompileTime];
				for (int i = 0; i < Traits::SizeAtCompileTime; ++i)
					rightCol[i] = right(i, Col);
				Scalar const* leftRow = left.data() + traits<MatrixL>::ColsAtCompileTime * Row;
				VectorizedDot<Scalar, PacketType, Traits::IsAligned,
					0, Traits::VectorizableSize>::Run(dst(Row, Col), leftRow, rightCol);
				DefaultDot<Scalar, Traits::VectorizableSize,
					Traits::SizeAtCompileTime>::Run(dst(Row, Col), leftRow, rightCol);
			}
			else
			{
				Scalar leftRow[Traits::SizeAtCompileTime];
				for (int i = 0; i < Traits::SizeAtCompileTime; ++i)
					leftRow[i] = left(Row, i);
				Scalar const* rightCol = right.data() + traits<MatrixR>::RowsAtCompileTime * Col;
				VectorizedDot<Scalar, PacketType, Traits::IsAligned,
					0, Traits::VectorizableSize>::Run(dst(Row, Col), leftRow, rightCol);
				DefaultDot<Scalar, Traits::VectorizableSize,
					Traits::SizeAtCompileTime>::Run(dst(Row, Col), leftRow, rightCol);
			}

			VectorizedMultiple<MatrixL, MatrixR, Index + 1, Stop>::Run(dst, left, right);
		}
	};

	template<typename MatrixL, typename MatrixR, int Stop>
	struct VectorizedMultiple<MatrixL, MatrixR, Stop, Stop>
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;
		static void Run(MatrixType&, const MatrixL&, const MatrixR&) {}
	};

	template<typename MatrixL, typename MatrixR> 
		requires MatricesMultiplable<MatrixL, MatrixR>
	struct Multiple
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;

		static MatrixType Run(const MatrixL& left, const MatrixR& right)
		{
			MatrixType dst;
			if constexpr (SUPPORT_SIMD)
				VectorizedMultiple<MatrixL, MatrixR, 0, traits<MatrixType>::SizeAtCompileTime>::Run(dst, left, right);
			else
				DefaultMultiple<MatrixL, MatrixR, 0, traits<MatrixType>::SizeAtCompileTime>::Run(dst, left, right);
			return dst;
		}
	};
}