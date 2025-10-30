/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Geometry/ConvexPolygon.h"
#include "Exception/BaseException.h"

namespace TG::Math::Geometry
{
    ConvexPolygon::ConvexPolygon(const Plane &plane, const std::vector<Vector3f>& vertices)
        : m_plane{ plane }, m_vertices{ vertices }
    {
        if (m_vertices.size() < 3)
            throw BaseException::Create("Vertices are less than 3");

        if (!IsAllPointsOnPlane())
            throw BaseException::Create("Vertices are not all on plane");

        MergeVertices();

        if (!IsConvex())
            throw BaseException::Create("The polygon that formed by vertices is not convex");
    }

    std::size_t ConvexPolygon::VertexCount() const
    {
        return m_vertices.size();
    }

    const Plane& ConvexPolygon::GetPlane() const
    {
        return m_plane;
    }

    bool ConvexPolygon::IsAllPointsOnPlane() const
    {
        return std::ranges::all_of(m_vertices, [this](const Vector3f& vertex) {
            if (m_plane.Distance(vertex) > EPSILON)
                return false;
            return true;
        });
    }

    /**
     * \brief 合并共线的顶点
     */
    void ConvexPolygon::MergeVertices()
    {
        std::vector mergedVertices{ m_vertices[0], m_vertices[1] };
        Vector3f segment{ m_vertices[1] - m_vertices[0] };
        Vector3f mergedVertex{ m_vertices[1] };
        for (int i = 2; i < m_vertices.size(); ++i)
        {
            if (segment.Cross(m_vertices[i] - mergedVertex).SquaredNorm() < EPSILON)
                continue;

            segment = m_vertices[i] - mergedVertex;
            mergedVertex = m_vertices[i];
            mergedVertices.emplace_back(mergedVertex);
        }
        std::size_t count = mergedVertices.size();
        segment = mergedVertices[0] - mergedVertices[count - 1];
        if (segment.Cross(mergedVertices[count - 1] - mergedVertices[count - 2]).SquaredNorm() < EPSILON)
            mergedVertices.pop_back();
        if (segment.Cross(mergedVertices[1] - mergedVertices[0]).SquaredNorm() < EPSILON)
            mergedVertices.erase(mergedVertices.begin());
        m_vertices.swap(mergedVertices);
    }

    bool ConvexPolygon::IsConvex() const
    {
        Vector3f a = m_vertices[0];
        Vector3f b = m_vertices[1];
        Vector3f c = m_vertices[2];
        const Vector3f convexDirection = (b - a).Cross(c - b);

        const int n = static_cast<int>(m_vertices.size());
        for (int i = 1; i < n; ++i)
        {
            a = m_vertices[i];
            b = m_vertices[(i + 1) % n];
            c = m_vertices[(i + 2) % n];

            if (Vector3f direction = (b - a).Cross(c - b); convexDirection.Dot(direction) <= 0)
                return false;
        }
        return true;
    }

    bool ConvexPolygon::Contains(const Vector3f& point) const
    {
        const Vector3f direction = (m_vertices[1] - m_vertices[0]).Cross(point - m_vertices[0]);
        const int n = static_cast<int>(m_vertices.size());
        for (int i = 1; i < n; ++i)
        {
            Vector3f ab = m_vertices[(i + 1) % n] - m_vertices[i];
            Vector3f ap = point - m_vertices[i];

            Vector3f currentDirection = ab.Cross(ap);
            if (currentDirection.SquaredNorm() < EPSILON)
                continue;

            if (direction.Dot(currentDirection) < 0)
                return false;
        }
        return true;
    }
}
