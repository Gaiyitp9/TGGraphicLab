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
	private:
		constexpr static int row_major_bit = Options_ & RowMajor ? RowMajorBit : 0;
		constexpr static int packet_access_bit = packet_traits<Scalar_>::Vectorizable ? PacketAccessBit : 0;

	public:
		using Scalar = Scalar_;
		constexpr static int Size = size_at_compile_time(Rows, Cols);
		using PacketScalar = find_best_packet<Scalar_, Size>::type;
		constexpr static int RowsAtCompileTime = Rows;
		constexpr static int ColsAtCompileTime = Cols;
		constexpr static int Options = Options_;
		constexpr static int Flags = DirectAccessBit | LvalueBit | NestByRefBit | row_major_bit;
		constexpr static int Alignment = unpacket_traits<PacketScalar>::Alignment;
		constexpr static int EvaluatorFlags = LinearAccessBit | DirectAccessBit | row_major_bit | packet_access_bit;
	};

	template<typename Scalar_, int Rows, int Cols, int Options_>
	class Matrix : public MatrixBase<Matrix<Scalar_, Rows, Cols, Options_>>
	{
	public:
		using Base = MatrixBase<Matrix>;
		using typename Base::Scalar;
		using Base::RowsAtCompileTime;
		using Base::ColsAtCompileTime;
		using Base::SizeAtCompileTime;
		using Base::IsVectorAtCompileTime;
		constexpr static int Alignment = traits<Matrix>::Alignment;

	public:
		int rows() const { return m_storage.rows(); }
		int cols() const { return m_storage.cols(); }
		int size() const { return m_storage.size(); }

		const Scalar& operator[](size_t index) const { return m_storage[index]; }
		Scalar& operator[](size_t index) { return m_storage[index]; }

		template<typename OtherDerived>
		Matrix& operator=(const MatrixBase<OtherDerived>& other)
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

	using Vectorm3f = Matrix<float, 1, 3>;
}