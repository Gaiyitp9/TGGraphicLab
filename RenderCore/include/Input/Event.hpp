/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Device.hpp"
#include "EventData.hpp"
#include <format>
#include <unordered_map>
#include <variant>

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

    // https://stackoverflow.com/questions/45575892/why-is-sizeofstdvariant-the-same-size-as-a-struct-with-the-same-members
    // 关于variant的大小问题，因为variant需要额外的字节来记录哪个类型被实例化，所以比union要多一些字节，多的字节需要根据padding计算
	struct EventInfo
	{
		static std::unordered_map<KeyCode, wchar_t const*> keysName;
		static std::unordered_map<EventType, wchar_t const*> eventTypes;
	};
}