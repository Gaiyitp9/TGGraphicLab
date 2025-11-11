/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Mesh/Sphere.h"
#include <numbers>

namespace TG::Rendering
{
    Sphere::Sphere()
    {
        Regenerate();
    }

    Sphere::Sphere(float radius, int columns, int rows)
        : m_radius(radius), m_columns(columns), m_rows(rows)
    {
        Regenerate();
    }

    Sphere::~Sphere() = default;

    void Sphere::Regenerate()
    {
        const auto deltaTheta = static_cast<float>(std::numbers::pi / m_rows);
        const auto deltaPhi = static_cast<float>(std::numbers::pi * 2 / m_columns);

        for (int i = 0; i <= m_rows; ++i)
        {
            const float theta = deltaTheta * static_cast<float>(i);
            for (int j = 0; j <= m_columns; ++j)
            {
                const float phi = deltaPhi * static_cast<float>(j);
                Math::Vector3f v{
                    std::sin(theta) * std::cos(phi),
                    std::cos(theta),
                    std::sin(theta) * std::sin(phi)
                };

                vertices.emplace_back(m_radius * v);
                uvs.emplace_back(
                    static_cast<float>(j) / static_cast<float>(m_columns),
                    static_cast<float>(i) / static_cast<float>(m_rows)
                );
                normals.emplace_back(v);
                tangents.emplace_back();
                colors.push_back({ 1.0f, 1.0f, 1.0f });
            }
        }

        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_columns; ++j)
            {
                const int index0 = i * (m_columns + 1) + j;
                const int index1 = index0 + 1;
                const int index2 = (i + 1) * (m_columns + 1) + j;
                const int index3 = index2 + 1;
                indices.push_back(index0);
                indices.push_back(index3);
                indices.push_back(index2);
                indices.push_back(index0);
                indices.push_back(index1);
                indices.push_back(index3);

                // 每个顶点累加三角面的切线
                const Math::Vector3f tangent0 = CalculateTangent(
                    vertices[index0], uvs[index0],
                    vertices[index2], uvs[index2],
                    vertices[index3], uvs[index3]
                );
                tangents[index0] += tangent0;
                tangents[index2] += tangent0;
                tangents[index3] += tangent0;

                const Math::Vector3f tangent1 = CalculateTangent(
                    vertices[index0], uvs[index0],
                    vertices[index1], uvs[index1],
                    vertices[index3], uvs[index3]
                );
                tangents[index0] += tangent1;
                tangents[index1] += tangent1;
                tangents[index3] += tangent1;
            }
        }

        // 对切线执行Gram-Schmidt正交化
        for (int i = 0; i < vertices.size(); ++i)
            tangents[i] = (tangents[i] - normals[i] * normals[i].Dot(tangents[i])).Normalized();
    }

    Math::Vector3f Sphere::CalculateTangent(const Math::Vector3f& v1, const Math::Vector2f& uv1,
            const Math::Vector3f& v2, const Math::Vector2f& uv2,
            const Math::Vector3f& v3, const Math::Vector2f& uv3)
    {
        // Mikkt算法(也叫做MikktTSpace切线空间)
        // 理论上顶点的切平面上的线都是切线，图形学中规定切线平行于纹理坐标系的U轴，
        // 所以，切线是纹理坐标系的U轴，副切线是纹理坐标系的V轴
        // 设纹理坐标(u, v, 0, 1)对应的局部坐标为(x, y, z, 1)
        // 注意，切线和副切线在UV平面上，法线垂直UV平面，所以对应z坐标为0
        // 满足以下变换:
        // [x   [Tx Bx Nx Dx   [u
        //  y =  Ty By Ny Dy *  v
        //  z    Tz Bz Nz Dz    0
        //  1]   0  0  0  1]   1]
        // 其中T表示切线，B表示副切线，N表示法线，D表示位移
        // 对于每个分量，以x为例，可以得到x = u * Tx + v * Bx + Dx
        // 为了消除位移，计算两个顶点的UV坐标差值和局部坐标差值，对于每个分量，以x为例，可以得到:
        // Δx = Δu * Tx + Δv * Bx
        // 表达式包含两个变量，所以我们利用三个顶点v1、v2和v3，计算v2 - v1和v3 - v1，得到
        // [Δx1  = [Δu1, Δv1  * [Tx
        //  Δx2]    Δu2, Δv2]    Bx]
        // 等式两边左乘逆矩阵可以解出[Tx, Bx]。yz分量同上
        // 每个三角形能计算出一条切线，对于每个顶点来说，需要加上所有共用该顶点的三角形切线并执行Gram-Schmidt正交化
        Math::Vector3f deltaPosition1 = v2 - v1;
        Math::Vector3f deltaPosition2 = v3 - v1;
        Math::Vector2f deltaUV1 = uv2 - uv1;
        Math::Vector2f deltaUV2 = uv3 - uv1;

        return 1.0f / (deltaUV1.X() * deltaUV2.Y() - deltaUV1.Y() * deltaUV2.X()) *
            (deltaPosition1 * deltaUV2.Y() - deltaPosition2 * deltaUV1.Y());
    }
}
