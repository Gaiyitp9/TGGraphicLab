/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

// 二元运算求值器
template<typename T, 
		 typename LhsScalar = typename traits<typename T::LhsPlain>::Scalar,
		 typename RhsScalar = typename traits<typename T::RhsPlain>::Scalar>
struct binary_evaluator;

// 一元运算求值器
template<typename T, 
		 typename Scalar = typename T::Scalar>
struct unary_evaluator;

template<typename T>
struct evaluator_assume_aliasing
{
	constexpr static bool value = false;
};

template<typename T>
struct evaluator : public unary_evaluator<T>
{
	using Base = unary_evaluator<T>;
	explicit evaluator(const T& xpr) : Base(xpr) {}
};

template<typename Scalar_, int Rows, int Cols, StorageOption Options>
struct evaluator<Matrix<Scalar_, Rows, Cols, Options>>
{
	using XprType = Matrix<Scalar_, Rows, Cols, Options>;
	using Scalar = XprType::Scalar;
	using CoeffReturnType = XprType::CoeffReturnType;

	constexpr static bool IsRowMajor = XprType::IsRowMajor;
	constexpr static bool IsVectorAtCompileTime = XprType::IsVectorAtCompileTime;
	constexpr static int RowsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static int Flags = traits<XprType>::EvaluatorFlags;
	constexpr static int Alignment = traits<XprType>::Alignment;
	constexpr static int OuterStrideAtCompileTime = XprType::OuterStrideAtCompileTime;

	evaluator() : m_data(nullptr), m_outerStride(OuterStrideAtCompileTime) {}
	explicit evaluator(const XprType& m) : m_data(m.m_storage.data()), m_outerStride(m.outerStride()) {}

	CoeffReturnType coeff(int row, int col) const
	{
		if constexpr (IsRowMajor)
			return m_data[row * m_outerStride + col];
		else
			return m_data[row + col * m_outerStride];
	}

	CoeffReturnType coeff(int index) const
	{
		return m_data[index];
	}

	Scalar& coeffRef(int row, int col)
	{
		if constexpr (IsRowMajor)
			return const_cast<Scalar*>(m_data)[row * m_outerStride + col];
		else
			return const_cast<Scalar*>(m_data)[row + col * m_outerStride];
	}

	Scalar& coeffRef(int index)
	{
		return const_cast<Scalar*>(m_data)[index];
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		if constexpr (IsRowMajor)
			return ploadt<PacketType, LoadMode>(m_data + row * m_outerStride + col);
		else
			return ploadt<PacketType, LoadMode>(m_data + row + col * m_outerStride);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return ploadt<PacketType, LoadMode>(m_data + index);
	}

protected:
	const Scalar* m_data;
	int m_outerStride;
};

template<typename ArgType>
struct unary_evaluator<Transpose<ArgType>>
{
	using XprType = Transpose<ArgType>;
	using Scalar = XprType::Scalar;
	using CoeffReturnType = XprType::CoeffReturnType;
	constexpr static int RowsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static int Flags = evaluator<ArgType>::Flags ^ RowMajorBit;
	constexpr static int Alignment = evaluator<ArgType>::Alignment;

	explicit unary_evaluator(const XprType& t) : m_argImpl(t.nestedExpression()) {}

	CoeffReturnType coeff(int row, int col) const
	{
		return m_argImpl.coeff(col, row);
	}

	CoeffReturnType coeff(int index) const
	{
		return m_argImpl.coeff(index);
	}

	Scalar& coeffRef(int row, int col)
	{
		return m_argImpl.coeffRef(col, row);
	}

	Scalar& coeffRef(int index)
	{
		return m_argImpl.coeffRef(index);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		return m_argImpl.template packet<LoadMode, PacketType>(col, row);
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return m_argImpl.template packet<LoadMode, PacketType>(index);
	}

private:
	evaluator<ArgType> m_argImpl;
};

template<typename BinaryOp, typename Lhs, typename Rhs>
struct evaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
	: public binary_evaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
	using Base = binary_evaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>;
	using typename Base::XprType;

	explicit evaluator(const XprType& xpr) : Base(xpr) {}
};

template<typename BinaryOp, typename Lhs, typename Rhs>
struct binary_evaluator<CwiseBinaryOp<BinaryOp, Lhs, Rhs>>
{
	using XprType = CwiseBinaryOp<BinaryOp, Lhs, Rhs>;
	using CoeffReturnType = XprType::CoeffReturnType;

	constexpr static int LhsFlags = evaluator<Lhs>::Flags;
	constexpr static int RhsFlags = evaluator<Rhs>::Flags;
	constexpr static bool SameType = std::is_same_v<typename Lhs::Scalar, typename Rhs::Scalar>;
	constexpr static bool StorageOrdersAgree = (LhsFlags & RowMajorBit) == (RhsFlags & RowMajorBit);
	constexpr static int Alignment = evaluator<Lhs>::Alignment < evaluator<Rhs>::Alignment ? evaluator<Lhs>::Alignment
									: evaluator<Rhs>::Alignment;
	// 判断是否要开启LinearAccessBit和PacketAccessBit位
	constexpr static int Flags0 = (int(LhsFlags) | int(RhsFlags)) & (
		(int(LhsFlags) & int(RhsFlags) &
			((StorageOrdersAgree ? LinearAccessBit : 0)
				| (functor_traits<BinaryOp>::PacketAccess && StorageOrdersAgree && SameType ? PacketAccessBit : 0)
				)
			)
		);
	constexpr static int Flags = (Flags0 & ~RowMajorBit) | (LhsFlags & RowMajorBit);	// 取Lhs的RowMajorBit标志位

	explicit binary_evaluator(const XprType& xpr) : m_d(xpr) {}

	CoeffReturnType coeff(int row, int col) const
	{
		return m_d.func()(m_d.lhsImpl.coeff(row, col), m_d.rhsImpl.coeff(row, col));
	}

	CoeffReturnType coeff(int index) const
	{
		return m_d.func()(m_d.lhsImpl.coeff(index), m_d.rhsImpl.coeff(index));
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int row, int col) const
	{
		return m_d.func().packetOp(m_d.lhsImpl.template packet<LoadMode, PacketType>(row, col),
			m_d.rhsImpl.template packet<LoadMode, PacketType>(row, col));
	}

	template<int LoadMode, typename PacketType>
	PacketType packet(int index) const
	{
		return m_d.func().packetOp(m_d.lhsImpl.template packet<LoadMode, PacketType>(index),
			m_d.rhsImpl.template packet<LoadMode, PacketType>(index));
	}

protected:
	// this helper permits to completely eliminate the functor if it is empty
	struct Data
	{
		Data(const XprType& xpr) : op(xpr.functor()), lhsImpl(xpr.lhs()), rhsImpl(xpr.rhs()) {}
		const BinaryOp& func() const { return op; }
		BinaryOp op;
		evaluator<Lhs> lhsImpl;
		evaluator<Rhs> rhsImpl;
	};

	Data m_d;
};

}