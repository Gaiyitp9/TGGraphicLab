/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Geometry/Triangle.h"

namespace TG::Math::Geometry
{
    Triangle::Triangle(const Vector3f &pointA, const Vector3f &pointB, const Vector3f &pointC)
        : m_pointA(pointA), m_pointB(pointB), m_pointC(pointC)
    {

    }

    /**
     * \brief 判断点是否在三角形内部
     * \param point 与三角形在同一平面上的点
     * \return 是否相交
     */
    bool Triangle::Contains(const Vector3f& point) const
    {
        // 定义线段AB, BC, CA的内侧为包含三角形的那一侧
        // 当点P同时在AB, BC, CA的内侧，则点P在三角形内部
        // 对于向量AB, BC, CA以及AP, BP, CP
        // AB叉乘AP的向量，BC叉乘BP的向量，CA叉乘CP的向量方向必须相同，
        // 才能保证P在AB, BC, CA的同一侧，也就是内侧
        const Vector3f ab = m_pointB - m_pointA;
        const Vector3f bc = m_pointC - m_pointB;
        const Vector3f ca = m_pointA - m_pointC;

        const Vector3f ap = point - m_pointA;
        const Vector3f bp = point - m_pointB;
        const Vector3f cp = point - m_pointC;

        const Vector3f abp = ab.Cross(ap);
        const Vector3f bcp = bc.Cross(bp);
        const Vector3f cap = ca.Cross(cp);

        // 等于0表示在三角形边上
        return abp.Dot(bcp) >= 0 && abp.Dot(cap) >= 0 && bcp.Dot(cap) >= 0;
    }
}
