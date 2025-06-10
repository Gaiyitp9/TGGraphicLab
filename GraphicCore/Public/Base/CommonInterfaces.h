/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformTraits.h"

namespace TG
{
    // 显示接口
    template <typename PlatformTraits> requires IsValidPlatformTraits<PlatformTraits>
    struct IVideoPort
    {
        virtual ~IVideoPort() = default;

        [[nodiscard]] virtual PlatformTraits::WindowHandle Handle() const noexcept = 0;
        [[nodiscard]] virtual PlatformTraits::DeviceContext Context() const noexcept = 0;
        [[nodiscard]] virtual int Width() const noexcept = 0;
        [[nodiscard]] virtual int Height() const noexcept = 0;
    };

    using IDefaultVideoPort = IVideoPort<DefaultPlatformTraits>;

    // 计时器接口
    struct ITimer
    {
        virtual ~ITimer() = default;

        [[nodiscard]] virtual float TotalTime() const = 0;
        [[nodiscard]] virtual float DeltaTime() const = 0;
    };

    // 输入接口
    struct IInput
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
