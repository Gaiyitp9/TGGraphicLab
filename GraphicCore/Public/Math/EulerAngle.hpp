/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math
{
    // 欧拉角旋转顺序，这里以世界坐标系的轴作为旋转轴
    enum class RotationOrder : unsigned char
    {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX,
    };

    template<typename Scalar>
    class EulerAngle : public RotationBase<EulerAngle<Scalar>, 3>
    {
        using Base = RotationBase<Scalar, 3>;
        using RotationMatrix = Base::RotationMatrix;

    public:
        EulerAngle() : m_euler{ 0, 0, 0 } {}
        EulerAngle(Scalar x, Scalar y, Scalar z) : m_euler { x, y, z } {}
        explicit EulerAngle(const Vector<Scalar, 3>& euler) : m_euler(euler) {}

        RotationMatrix ToRotationMatrix() const
        {
            RotationMatrix result;
            const Scalar cp = std::cos(m_euler.X());
            const Scalar sp = std::sin(m_euler.X());
            const Scalar ch = std::cos(m_euler.Y());
            const Scalar sh = std::sin(m_euler.Y());
            const Scalar cb = std::cos(m_euler.Z());
            const Scalar sb = std::sin(m_euler.Z());
            result[0, 0] = ch * cp + sh * sp * sb;
            result[0, 1] = -ch * sb + sh * sp * cb;
            result[0, 2] = sh * cp;
            result[1, 0] = cp * sb;
            result[1, 1] = cb * cp;
            result[1, 2] = -sp;
            result[2, 0] = -cb * sh + ch * sp * sb;
            result[2, 1] = sb * sh + ch * sp * cb;
            result[2, 2] = ch * cp;
            return result;
        }


    private:
        Vector<Scalar, 3> m_euler;
        // 注：欧拉角计算四元数或矩阵需要指定旋转顺序和参考坐标系
        // 当按局部坐标系旋转和按世界坐标系旋转的轴旋转矩阵相乘顺序相反时，这两种旋转等价
        // 默认旋转顺序为heading-pitch-bank(yaw-pitch-roll，局部坐标系)
        // Z-X-Y(Object Space)或Y-X-Z(World Space 或 Parent Space)
        RotationOrder m_order{ RotationOrder::YXZ };
    };
}
