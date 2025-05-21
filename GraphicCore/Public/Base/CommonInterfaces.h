/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Policy.h"

namespace TG
{
    // 显示接口
    template <typename NativePolicy> requires IsValidNativePolicy<NativePolicy>
    struct IVideoPort
    {
        virtual ~IVideoPort() = default;

        [[nodiscard]] virtual NativePolicy::WindowHandle GetHandle() const noexcept = 0;
        [[nodiscard]] virtual NativePolicy::DeviceContext GetContext() const noexcept = 0;
        [[nodiscard]] virtual int Width() const noexcept = 0;
        [[nodiscard]] virtual int Height() const noexcept = 0;
    };

    using IDefaultVideoPort = IVideoPort<DefaultNativePolicy>;

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
