/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Transform/RotationBase.hpp"

namespace TG::Math
{
    template<typename Scalar>
    class AngleAxis final : public RotationBase<AngleAxis<Scalar>, 3>
    {
    public:
        AngleAxis() = default;

        template<typename Derived>
        AngleAxis(const Scalar& angle, const MatrixBase<Derived>& axis) : m_angle(angle), m_axis(axis)
        {
            m_axis.Normalize();
        }

        Matrix3<Scalar> ToRotationMatrix() const
        {
            // 罗德里格斯旋转公式(Rodrigues rotation formula)
            // 投影矩阵P = uuT
            Matrix3<Scalar> projectionMatrix = m_axis * m_axis.Transpose();
            // 叉乘矩阵
            Matrix3<Scalar> crossMatrix{
                0, -m_axis.Z(), m_axis.Y(),
                m_axis.Z(), 0, -m_axis.X(),
                -m_axis.Y(), m_axis.X(), 0,
            };
            Scalar s = std::sin(m_angle);
            Scalar c = std::cos(m_angle);

            return (static_cast<Scalar>(1) - c) * projectionMatrix +
                c * Matrix3<Scalar>::Identity() + s * crossMatrix;
        }

    private:
        Scalar m_angle{ 0 };
        Vector3<Scalar> m_axis{ 0, 1, 0 };
    };

    template<typename ScalarT>
    struct Traits<AngleAxis<ScalarT>>
    {
        using Scalar = ScalarT;
    };
}
