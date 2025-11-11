/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Transform/RotationBase.hpp"

namespace TG::Math
{
    template<typename Scalar, std::size_t Dimension>
    class Transform
    {
        static constexpr std::size_t HDimension = Dimension + 1;    // size of a respective homogeneous vector
        using MatrixType = Matrix<Scalar, HDimension, HDimension>;

    public:
        Transform() : m_matrix{ MatrixType::Identity() } {}

        const MatrixType& ToTransformMatrix() const noexcept { return m_matrix; }

        Block<MatrixType, Dimension, Dimension> LinearExt()
        {
            return m_matrix.template Block<Dimension, Dimension>(0, 0);
        }

        Block<const MatrixType, Dimension, Dimension> LinearExt() const
        {
            return m_matrix.template Block<Dimension, Dimension>(0, 0);
        }

        Block<MatrixType, Dimension, 1> TranslationExt()
        {
            return m_matrix.template Block<Dimension, 1>(0, Dimension);
        }

        Block<const MatrixType, Dimension, 1> TranslationExt() const
        {
            return m_matrix.template Block<Dimension, 1>(0, Dimension);
        }

        template<typename Derived>
        Transform& Translate(const MatrixBase<Derived>& translation)
        {
            TranslationExt() += LinearExt() * translation;
            return *this;
        }

        template<typename Derived>
        Transform& Rotate(const RotationBase<Derived, 3>& rotation)
        {
            LinearExt() *= rotation.ToRotationMatrix();
            return *this;
        }

        Transform& Scale(const Scalar& s)
        {
            LinearExt() *= s;
            return *this;
        }

        Transform& PreScale(const Scalar& s)
        {
            m_matrix.template Block<Dimension, HDimension>(0, 0) *= s;
            return *this;
        }

    private:
        MatrixType m_matrix;
    };

    using Transform3f = Transform<float, 3>;
    using Transform3d = Transform<double, 3>;
}
