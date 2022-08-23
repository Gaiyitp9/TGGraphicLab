/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename MatrixType>
struct traits<Transpose<MatrixType>> : public traits<MatrixType>
{
private:
	constexpr static int FlagsLvalueBit = is_lvalue<MatrixType>::value ? LvalueBit : 0;
	constexpr static int Flags0 = traits<MatrixType>::Flags & ~(LvalueBit | NestByRefBit);				// 移除LvalueBit和NestByRefBit标志位
	constexpr static int Flags1 = Flags0 | FlagsLvalueBit;

public:
	constexpr static int RowsAtCompileTime = MatrixType::ColsAtCompileTime;
	constexpr static int ColsAtCompileTime = MatrixType::RowsAtCompileTime;
	constexpr static int InnerStrideAtCompileTime = inner_stride_at_compile_time<MatrixType>::value;
	constexpr static int OuterStrideAtCompileTime = outer_stride_at_compile_time<MatrixType>::value;
	constexpr static int Flags = Flags1 ^ RowMajorBit;													// RowMajorBit取反
};

template<typename MatrixType>
class Transpose : public MatrixBase<Transpose<MatrixType>>
{
public:
	using MatrixTypeNested = ref_selector<MatrixType>::non_const_type;
	using NestedExpression = remove_all_t<MatrixType>;

	explicit Transpose(MatrixType& matrix) : m_matrix(matrix) {}

	constexpr int rows() const noexcept { return m_matrix.cols(); }
	constexpr int cols() const noexcept { return m_matrix.rows(); }
	const NestedExpression& nestedExpression() const { return m_matrix; }
	NestedExpression& nestedExpression() { return m_matrix; }

protected:
	MatrixTypeNested m_matrix;
};

}