/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Plane.h"
#include <vector>

namespace TG::Math::Geometry
{
    class ConvexPolygon
    {
    public:
        ConvexPolygon(const Plane& plane, const std::vector<Vector3f>& vertices);

        [[nodiscard]] std::size_t VertexCount() const;

        [[nodiscard]] const Plane& GetPlane() const;

        [[nodiscard]] bool Contains(const Vector3f& point) const;

    private:
        [[nodiscard]] bool IsAllPointsOnPlane() const;
        void MergeVertices();
        [[nodiscard]] bool IsConvex() const;

        // 多边形所在平面
        Plane m_plane;
        // 顶点按顺时针或逆时针顺序排列
        std::vector<Vector3f> m_vertices;
        static constexpr float EPSILON = 1e-4f;
    };
}
