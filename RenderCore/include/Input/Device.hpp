/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.hpp"

namespace TG::Input
{
    // 设备类型
    enum class DeviceType : unsigned char
    {
        None,
        Mouse,
        Keyboard,
        Gamepad,
    };

    struct Event;

    // 输入设备的接口
    struct Device
    {
        virtual ~Device() = default;

        virtual void Update() = 0;                  // 更新设备状态
        virtual void Receive(const Event& e) = 0;   // 接收输入事件
        virtual void SpyEvent(bool enable) = 0;     // 监控输入事件
        virtual bool GetKey(KeyCode k) = 0;
        virtual bool GetKeyDown(KeyCode k) = 0;
        virtual bool GetKeyUp(KeyCode k) = 0;
    };
}