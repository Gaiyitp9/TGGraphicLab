/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order, bool IsConst>
    class Evaluator<Matrix<Scalar, Rows, Columns, Order>, IsConst>
    {
        using Xpr = Matrix<Scalar, Rows, Columns, Order>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;

    public:
        explicit Evaluator(InternalXpr& matrix) : m_matrix(matrix) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const
        {
            return m_matrix[index];
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_matrix[row, column];
        }

        Scalar& Entry(std::size_t index) requires (!IsConst)
        {
            return m_matrix[index];
        }

        Scalar& Entry(std::size_t row, std::size_t column) requires (!IsConst)
        {
            return m_matrix[row, column];
        }

    private:
        InternalXpr& m_matrix;
    };
}
