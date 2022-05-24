/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.hpp"
#include <format>
#include <unordered_map>

namespace LCH
{
	struct InputEvent
	{
		enum class Type
		{
			Press,
			Release,
			MouseMove,
			WheelRoll,
		};

		KeyCode key;
		Type type;
	};

	struct InputEventInfo
	{
		static std::unordered_map<LCH::KeyCode, wchar_t const*> keysName;
		static std::unordered_map<LCH::InputEvent::Type, wchar_t const*> eventTypes;
	};
}

template<typename CharT>
struct std::formatter<LCH::InputEvent, CharT>
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
	auto format(const LCH::InputEvent& e, FormatContext& ctx) const
	{
		switch (outputType)
		{
		case OutputType::KeyOnly:
			if (LCH::InputEventInfo::keysName.contains(e.key))
				return std::format_to(ctx.out(), L"Key: {}", LCH::InputEventInfo::keysName[e.key]);
			else
				return std::format_to(ctx.out(), L"Invalid KeyCode");

		case OutputType::EventOnly:
			if (LCH::InputEventInfo::eventTypes.contains(e.type))
				return std::format_to(ctx.out(), L"Event: {}", LCH::InputEventInfo::eventTypes[e.type]);
			else
				return std::format_to(ctx.out(), L"Invalid Event");

		default:
			if (LCH::InputEventInfo::keysName.contains(e.key) &&
				LCH::InputEventInfo::eventTypes.contains(e.type))
				return std::format_to(ctx.out(), L"Key: {}\tEvent: {}",
					LCH::InputEventInfo::keysName[e.key], LCH::InputEventInfo::eventTypes[e.type]);
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