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
	constexpr static int row_major_bit = (Options_ == StorageOption::RowMajor) ? RowMajorBit : 0;
	constexpr static int packet_access_bit = packet_traits<Scalar_>::Vectorizable ? PacketAccessBit : 0;
public:
	using Scalar = Scalar_;
	using PacketScalar = find_best_packet<Scalar_, size>::type;
	constexpr static int RowsAtCompileTime = Rows;
	constexpr static int ColsAtCompileTime = Cols;
	constexpr static int Flags = DirectAccessBit | LvalueBit | NestByRefBit | row_major_bit;
	constexpr static int InnerStrideAtCompileTime = 1;
	constexpr static int OuterStrideAtCompileTime = (Options_ == StorageOption::RowMajor) ? ColsAtCompileTime : RowsAtCompileTime;
	constexpr static int EvaluatorFlags = LinearAccessBit | DirectAccessBit | packet_access_bit | row_major_bit;
	constexpr static int Alignment = compute_default_alignment<Scalar_, size>::value;
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

	const Scalar* data() const { return m_storage.data(); }
	Scalar* data() { return m_storage.data(); }

	constexpr int innerStride() const noexcept { return 1; }
	constexpr int outerStride() const { return this->innerSize(); }

private:
	Storage<Scalar, SizeAtCompileTime, RowsAtCompileTime, ColsAtCompileTime> m_storage;
};

using Vector4f = Matrix<float, 1, 4>;
using Vector2i = Matrix<int, 1, 2>;

}