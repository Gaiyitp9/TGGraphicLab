/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "EventHandler.h"
#include <vector>

namespace TG::Input
{
    // 输入事件分发器
    class EventDispatcher
    {
    public:
        void Dispatch(const Event& event) const;
        void Register(IEventHandler& handler);
        void UnRegister(const IEventHandler& handler);

    private:
        std::vector<std::reference_wrapper<IEventHandler>> m_handlers;
    };
}
