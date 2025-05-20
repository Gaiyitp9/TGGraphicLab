/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Scalar, std::size_t Dimension, StorageOrder Order>
    class Transform
    {
        static constexpr std::size_t HDimension = Dimension + 1;    // size of a respective homogeneous vector
        using MatrixType = Matrix<Scalar, HDimension, HDimension, Order>;

    public:
        Transform()
        {
            m_matrix = MatrixType::Identity();
        }

        const MatrixType& ToTransformMatrix() const noexcept { return m_matrix; }

        Block<MatrixType, HDimension, Dimension> LinearExt()
        {
            return m_matrix.Block<HDimension, Dimension>(0, 0);
        }

        Block<const MatrixType, HDimension, Dimension> LinearExt() const
        {
            return m_matrix.Block<HDimension, Dimension>(0, 0);
        }

        Block<MatrixType, HDimension, 1> TranslationExt()
        {
            return m_matrix.Block<HDimension, 1>(0, Dimension);
        }

        Block<const MatrixType, HDimension, 1> TranslationExt() const
        {
            return m_matrix.Block<HDimension, 1>(0, Dimension);
        }

        template<typename Derived>
        Transform& Translate(const MatrixBase<Derived>& translation)
        {
            TranslationExt() = TranslationExt() + LinearExt() * translation;
            return *this;
        }

        template<typename Derived>
        Transform& Rotate(const RotationBase<Derived, 3>& rotation)
        {
            LinearExt() = LinearExt() * rotation.ToRotationMatrix();
            return *this;
        }

    private:
        MatrixType m_matrix;
    };
}
