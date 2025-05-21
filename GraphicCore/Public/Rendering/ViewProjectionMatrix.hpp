/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Rendering
{
    /**
     * \brief 计算相机矩阵。从世界空间变换到相机空间，世界空间和相机空间使用右手坐标系
     * \param eye     相机位置
     * \param center  注视点
     * \param worldUp 上方向
     */
    template<typename Scalar>
    Math::Matrix4<Scalar> LookAt(const Math::Vector3<Scalar>& eye, const Math::Vector3<Scalar>& center,
        const Math::Vector3<Scalar>& worldUp = { 0, 1, 0 })
    {
        Math::Matrix4<Scalar> view;
        // 这里计算出的front, right和up是左手坐标系，然后通过
        //          [1  0  0  0
        //           0  1  0  0
        //           0  0 -1  0
        //           0  0  0  1]
        // 转换为右手坐标系，这样相机的负方向就会指向center
        Math::Vector3<Scalar> front = (center - eye).Normalized();
        Math::Vector3<Scalar> right = front.Cross(worldUp).Normalized();
        Math::Vector3<Scalar> up = right.Cross(front);
        view.Block<1, 3>(0, 0) = right.Transpose();
        view.Block<1, 3>(1, 0) = up.Transpose();
        view[2, 0] = -front[0];
        view[2, 1] = -front[1];
        view[2, 2] = -front[2];
        view[0, 3] = -eye.Dot(right);
        view[1, 3] = -eye.Dot(up);
        view[2, 3] = eye.Dot(front);
        view[3, 3] = static_cast<Scalar>(1);
        return view;
    }

    /**
     * \brief 透视投影矩阵。从相机空间变换到裁剪空间，xyz坐标范围在[-1, 1]，裁剪空间使用左手坐标系
     * \param fov     相机视角(field of view)，这里使用的是垂直fov
     * \param aspect  长宽比
     * \param zNear   近平面位置，数值大于0
     * \param zFar    远平面位置，同上
     */
    template<typename Scalar>
    Math::Matrix4<Scalar> Perspective(Scalar fov, Scalar aspect, Scalar zNear, Scalar zFar)
    {
        Math::Matrix4<Scalar> perspective;
        Scalar tanHalfFov = std::tan(fov / static_cast<Scalar>(2));
        perspective[0, 0] = static_cast<Scalar>(1) / (aspect * tanHalfFov);
        perspective[1, 1] = static_cast<Scalar>(1) / (tanHalfFov);
        perspective[2, 2] = -(zFar + zNear) / (zFar - zNear);
        perspective[3, 2] = -static_cast<Scalar>(1);
        perspective[2, 3] = -static_cast<Scalar>(2) * zFar * zNear / (zFar - zNear);
        return perspective;
    }

    /**
     * \brief 正交投影矩阵，与透视投影矩阵类似
     * \param left    左边界
     * \param right   右边界
     * \param bottom  下边界
     * \param top     上边界
     * \param zNear   近平面，数值大于0
     * \param zFar    远平面
     */
    template<typename Scalar>
    Math::Matrix4<Scalar> Orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar zNear, Scalar zFar)
    {
        Math::Matrix4<Scalar> ortho;
        ortho[0, 0] = static_cast<Scalar>(2) / (right - left);
        ortho[1, 1] = static_cast<Scalar>(2) / (top - bottom);
        ortho[2, 2] = -static_cast<Scalar>(2) / (zFar - zNear);
        ortho[3, 3] = static_cast<Scalar>(1);
        ortho[0, 3] = -(right + left) / (right - left);
        ortho[1, 3] = -(top + bottom) / (top - bottom);
        ortho[2, 3] = -(zFar + zNear) / (zFar - zNear);
        return ortho;
    }
}
