/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.h"

namespace TG::Input
{
    // 输入事件处理器
    // 比如输入管理器，读取输入事件来更新输入设备状态
    struct IEventHandler
    {
        virtual ~IEventHandler() = default;
        virtual void Consume(const Event& event) = 0;
    };
}
