/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename Scalar_, int Rows, int Cols, StorageOption Options_>
struct traits<Matrix<Scalar_, Rows, Cols, Options_>>
{
private:
	constexpr static int size = size_at_compile_time(Rows, Cols);
	constexpr static Flag rowMajor = (Options_ == StorageOption::RowMajor) ? Flag::RowMajor : Flag::None;
	constexpr static Flag packetAccess = packet_traits<Scalar_>::Vectorizable ? Flag::PacketAccess : Flag::None;
public:
	using Scalar = Scalar_;
	using PacketScalar = best_packet<Scalar_, size>;
	constexpr static int RowsAtCompileTime = Rows;
	constexpr static int ColsAtCompileTime = Cols;
	constexpr static Flag Flags = Flag::DirectAccess | Flag::Lvalue | Flag::NestByRef | rowMajor;
	constexpr static int InnerStrideAtCompileTime = 1;
	constexpr static int OuterStrideAtCompileTime = (Options_ == StorageOption::RowMajor) ? ColsAtCompileTime : RowsAtCompileTime;
	constexpr static Flag EvaluatorFlags = Flag::LinearAccess | Flag::DirectAccess | packetAccess | rowMajor;
	constexpr static int Alignment = default_alignment<Scalar_, size>;
};

template<typename Scalar_, int Rows, int Cols, StorageOption Options_>
class Matrix : public MatrixBase<Matrix<Scalar_, Rows, Cols, Options_>>
{
public:
	using Base = MatrixBase<Matrix>;
	using typename Base::Scalar;
	using Base::RowsAtCompileTime;
	using Base::ColsAtCompileTime;
	using Base::SizeAtCompileTime;

public:
	constexpr int rows() const { return m_storage.rows(); }
	constexpr int cols() const { return m_storage.cols(); }

	template<typename OtherDerived>
	Matrix& operator=(const MatrixBase<OtherDerived>& other)
	{
		call_assignment(this->derived(), other.derived());
		return this->derived();
	}

	constexpr int innerStride() const noexcept { return 1; }
	constexpr int outerStride() const { return this->innerSize(); }

private:
	Storage<Scalar, SizeAtCompileTime, RowsAtCompileTime, ColsAtCompileTime> m_storage;

	friend struct evaluator<Matrix>;
};

#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
using Vector##SizeSuffix##TypeSuffix	= Matrix<Type, Size, 1>;	\
using RowVector##SizeSuffix##TypeSuffix = Matrix<Type, 1, Size>;	\
using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

#define MATRIX_ALLSIZE_TYPEDEF(Type, TypeSuffix)	\
MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)				\
MATRIX_TYPEDEF(Type, TypeSuffix, Dynamic, X)		

MATRIX_ALLSIZE_TYPEDEF(float, f)
MATRIX_ALLSIZE_TYPEDEF(double, d)
MATRIX_ALLSIZE_TYPEDEF(int, i)
}