/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mouse.hpp"
#include "Keyboard.hpp"
#include <memory>

namespace TG::Input
{
    // 输入设备创建工厂
    class DeviceFactory
    {
    public:
        static std::unique_ptr<Device> Create(DeviceType type)
        {
            switch (type)
            {
                case DeviceType::Mouse:
                    return std::make_unique<Mouse>();

                case DeviceType::Keyboard:
                    return std::make_unique<Keyboard>();

                case DeviceType::Gamepad:
                default:
                    return nullptr;
            }
        }
    };
}