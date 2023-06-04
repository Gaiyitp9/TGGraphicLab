/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Device.hpp"
#include <unordered_map>

namespace TG::Input
{
    // 输入事件类型
    enum class EventType : unsigned char
    {
        None,
        Press,
        Release,
        Char,
        MouseMove,
        WheelRoll,
    };

    // 输入事件
	struct Event
	{
        DeviceType device   = DeviceType::None;     // 输入设备类型
		KeyCode key         = KeyCode::None;        // 输入按键码
		EventType type      = EventType::None;      // 输入事件类型
        void* data          = nullptr;              // 设备相关数据
	};

	struct EventInfo
	{
		static std::unordered_map<KeyCode, char const*> keysName;
		static std::unordered_map<EventType, char const*> eventTypes;
	};
}