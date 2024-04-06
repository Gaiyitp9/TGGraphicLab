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
	concept MatricesMultiplable = Traits<MatrixL>::ColsAtCompileTime == Traits<MatrixR>::RowsAtCompileTime
                                  && std::is_same_v<typename Traits<MatrixL>::Scalar, typename Traits<MatrixR>::Scalar>
                                  && Traits<MatrixL>::IsRowMajor == Traits<MatrixR>::IsRowMajor;

	template<typename MatrixL, typename MatrixR>
		requires MatricesMultiplable<MatrixL, MatrixR>
	struct MatricesMultipleTraits
	{
	private:
		constexpr static StorageOption Option = Traits<MatrixL>::IsRowMajor ? StorageOption::RowMajor
                                                                            : StorageOption::ColumnMajor;

	public:
		using ReturnType = Matrix<
			typename Traits<MatrixL>::Scalar,
			Traits<MatrixL>::RowsAtCompileTime,
			Traits<MatrixR>::ColsAtCompileTime,
			Option
		>;
		constexpr static int Stride = Traits<ReturnType>::IsRowMajor
			? Traits<ReturnType>::ColsAtCompileTime
			: Traits<ReturnType>::RowsAtCompileTime;
		using Scalar = Traits<ReturnType>::Scalar;
		constexpr static int SizeAtCompileTime = Traits<MatrixL>::ColsAtCompileTime;
	};

	template<typename MatrixL, typename MatrixR, int Index, int Stop>
	struct DefaultMultiple
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;
		using Scalar = Traits<MatrixType>::Scalar;
		constexpr static int Row = Index / MatricesMultipleTraits<MatrixL, MatrixR>::Stride;
		constexpr static int Col = Index % MatricesMultipleTraits<MatrixL, MatrixR>::Stride;

		static void Run(MatrixType& dst, const MatrixL& left, const MatrixR& right)
		{
			Scalar sum = 0;
			for (int i = 0; i < Traits<MatrixL>::ColsAtCompileTime; ++i)
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

	template<typename MatrixL, typename MatrixR> 
		requires MatricesMultiplable<MatrixL, MatrixR>
	struct Multiple
	{
		using MatrixType = MatricesMultipleTraits<MatrixL, MatrixR>::ReturnType;

		static MatrixType Run(const MatrixL& left, const MatrixR& right)
		{
			MatrixType dst;
            DefaultMultiple<MatrixL, MatrixR, 0, Traits<MatrixType>::SizeAtCompileTime>::Run(dst, left, right);
			return dst;
		}
	};
}