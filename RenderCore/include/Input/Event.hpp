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
    // https://stackoverflow.com/questions/45575892/why-is-sizeofstdvariant-the-same-size-as-a-struct-with-the-same-members
    // 关于variant的大小问题，因为variant需要额外的字节来记录哪个类型被实例化，所以比union要多一些字节，多的字节需要根据padding计算
	struct Event
	{
        DeviceType device;                              // 输入设备类型
		KeyCode key;                                    // 输入按键码
		EventType type;                                 // 输入事件类型
        std::variant<MouseData, KeyboardData> data;     // 设备相关数据
	};

	struct EventInfo
	{
		static std::unordered_map<KeyCode, wchar_t const*> keysName;
		static std::unordered_map<EventType, wchar_t const*> eventTypes;
	};
}

template<typename CharT>
struct std::formatter<TG::Input::Event, CharT>
{
	constexpr auto parse(basic_format_parse_context<CharT>& ctx)
	{
		auto iter{ ctx.begin() };
		const auto end{ ctx.end() };
		if (iter == end || *iter == '}')
		{
			outputType = OutputType::KeyAndEvent;
			return iter;
		}
		switch (*iter)
		{
		case 'k':
			outputType = OutputType::KeyOnly;
			break;
		case 'e':
			outputType = OutputType::EventOnly;
			break;
		default:
			throw format_error{ "Invalid KeyValue format specifier" };
		}

		++iter;
		if (iter != end && *iter != '}')
			throw format_error{ "Invalid KeyValue format specifier" };
		return iter;
	}

	template<typename FormatContext>
	auto format(const TG::Input::Event& e, FormatContext& ctx) const
	{
		switch (outputType)
		{
		case OutputType::KeyOnly:
			if (TG::Input::EventInfo::keysName.contains(e.key))
				return std::format_to(ctx.out(), L"Key: {}", TG::Input::EventInfo::keysName[e.key]);
			else
				return std::format_to(ctx.out(), L"Invalid KeyCode");

		case OutputType::EventOnly:
			if (TG::Input::EventInfo::eventTypes.contains(e.type))
				return std::format_to(ctx.out(), L"Event: {}", TG::Input::EventInfo::eventTypes[e.type]);
			else
				return std::format_to(ctx.out(), L"Invalid Event");

		default:
			if (TG::Input::EventInfo::keysName.contains(e.key) &&
				TG::Input::EventInfo::eventTypes.contains(e.type))
				return std::format_to(ctx.out(), L"Key: {}\tEvent: {}",
					TG::Input::EventInfo::keysName[e.key], TG::Input::EventInfo::eventTypes[e.type]);
			else
				return std::format_to(ctx.out(), L"Invalid Input Event");
		}
	}

private:
	enum class OutputType
	{
		KeyOnly,
		EventOnly,
		KeyAndEvent,
	};
	OutputType outputType{ OutputType::KeyAndEvent };
};