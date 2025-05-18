/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Rendering
{
    // 投影矩阵。从相机空间变换到裁剪空间，xyz坐标范围在[-1, 1]
    // 裁剪空间使用左手坐标系，世界空间和相机空间使用右手坐标系

    /**
     * 透视投影矩阵
     * @fov     相机视角(field of view)，这里使用的是垂直fov
     * @aspect  长宽比
     * @zNear   近平面位置，数值大于0
     * @zFar    远平面位置，同上
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
     * 正交投影矩阵
     * 投影正交视锥内的物体
     * @left    左边界
     * @right   右边界
     * @bottom  下边界
     * @top     上边界
     * @zNear   近平面，数值大于0
     * @zFar    远平面
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
