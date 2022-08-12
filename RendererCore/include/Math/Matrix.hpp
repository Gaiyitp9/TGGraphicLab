/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Scalar_, int Rows, int Cols, int Options_>
	struct traits<Matrix<Scalar_, Rows, Cols, Options_>>
	{
	public:
		using Scalar = Scalar_;
		constexpr static int Size = (Rows == Dynamic || Cols == Dynamic) ? Dynamic : Rows * Cols;
		constexpr static int RowsAtCompileTime = Rows;
		constexpr static int ColsAtCompileTime = Cols;
		constexpr static int Options = Options_;
	};

	template<typename Scalar_, int Rows, int Cols, int Options_>
	class Matrix : public MatrixBase<Matrix<Scalar_, Rows, Cols, Options_>>
	{
		using Base = MatrixBase<Matrix>;
		using Base::Scalar;
		using Base::RowsAtCompileTime;
		using Base::ColsAtCompileTime;
		using Base::SizeAtCompileTime;
		using Base::IsVectorAtCompileTime;
		using Base::Options;

	private:
		Storage<Scalar, SizeAtCompileTime, RowsAtCompileTime, ColsAtCompileTime, Options> storage;
	};
}