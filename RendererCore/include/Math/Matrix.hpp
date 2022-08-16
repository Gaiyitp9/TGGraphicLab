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
		constexpr static int Flags = NestByRefBit;
		constexpr static int Alignment = (Options & DontAlign) ? SimdInstruction<Scalar, Size>::Alignment : 0;
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
		using Base::Alignment;

	public:
		int rows() const { return m_storage.rows(); }
		int cols() const { return m_storage.cols(); }
		int size() const { return m_storage.size(); }

		const Scalar& operator[](size_t index) const { return m_storage[index]; }
		Scalar& operator[](size_t index) const { return m_storage[index]; }

		Matrix& operator=(const MatrixBase& other)
		{
			for (int i = 0; i < SizeAtCompileTime; ++i)
			{
				m_storage[i] = other.derived()[i];
			}
			return *this;
		}

	private:
		Storage<Scalar, SizeAtCompileTime, RowsAtCompileTime, ColsAtCompileTime, Alignment> m_storage;
	};

	using Vector3f = Matrix<float, 1, 3>;
}