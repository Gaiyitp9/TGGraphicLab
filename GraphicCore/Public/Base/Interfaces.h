/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformTraits.hpp"

namespace TG
{
    // 显示接口
    struct TG_API IVideoPort
    {
        using PlatformTraitsT = PlatformTraits<g_platform>;

        virtual ~IVideoPort() = default;

        [[nodiscard]] virtual PlatformTraitsT::WindowHandle Handle() const noexcept = 0;
        [[nodiscard]] virtual PlatformTraitsT::RenderSurface Surface() const noexcept = 0;
        [[nodiscard]] virtual unsigned int Width() const noexcept = 0;
        [[nodiscard]] virtual unsigned int Height() const noexcept = 0;
    };

    // 计时器接口
    struct TG_API ITimer
    {
        virtual ~ITimer() = default;

        [[nodiscard]] virtual float TotalTime() const = 0;
        [[nodiscard]] virtual float DeltaTime() const = 0;
    };

    // 输入接口
    struct TG_API IInput
    {
        virtual ~IInput() = default;

        [[nodiscard]] virtual bool GetKey(Input::KeyCode key) const = 0;
        [[nodiscard]] virtual bool GetKeyDown(Input::KeyCode key) const = 0;
        [[nodiscard]] virtual bool GetKeyUp(Input::KeyCode key) const = 0;
        [[nodiscard]] virtual short MousePositionX() const = 0;
        [[nodiscard]] virtual short MousePositionY() const = 0;
        [[nodiscard]] virtual short MouseWheelDelta() const = 0;
    };
}
