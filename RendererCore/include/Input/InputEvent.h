/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
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

}

namespace std
{
	template<typename CharT>
	struct std::formatter<LCH::InputEvent, CharT>
	{
		template<typename FormatParseContext>
		auto parse(FormatParseContext& pc)
		{
			auto iter{ pc.begin() };
			const auto end{ pc.end() };
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
		auto format(const LCH::InputEvent& e, FormatContext& fc)
		{
			switch (outputType)
			{
			case OutputType::KeyOnly:
				if (keysName.contains(e.key))
					return std::format_to(fc.out(), L"Key: {}", keysName[e.key]);
				else
					return std::format_to(fc.out(), L"Invalid KeyCode");

			case OutputType::EventOnly:
				if (types.contains(e.type))
					return std::format_to(fc.out(), L"Event: {}", types[e.type]);
				else
					return std::format_to(fc.out(), L"Invalid Event");

			default:
				if (keysName.contains(e.key) && types.contains(e.type))
					return std::format_to(fc.out(), L"Key: {}\tEvent: {}", keysName[e.key], types[e.type]);
				else
					return std::format_to(fc.out(), L"Invalid Input Event");
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

	private:
		std::unordered_map<LCH::KeyCode, wchar_t const*> keysName =
		{
			{LCH::KeyCode::None, L"None"},
			{LCH::KeyCode::LeftMouseButton, L"LeftMouseButton"},
			{LCH::KeyCode::RightMouseButton, L"RightMouseButton"},
			{LCH::KeyCode::MidMouseButton, L"MidMouseButton"},
			{LCH::KeyCode::Backspace, L"Backspace"},
			{LCH::KeyCode::Tab, L"Tab"},
			{LCH::KeyCode::Enter, L"Enter"},
			{LCH::KeyCode::Esc, L"Esc"},
			{LCH::KeyCode::Space, L"Space"},
			{LCH::KeyCode::PageUp, L"PageUp"},
			{LCH::KeyCode::PageDown, L"PageDown"},
			{LCH::KeyCode::End, L"End"},
			{LCH::KeyCode::Home, L"Home"},
			{LCH::KeyCode::LeftArrow, L"LeftArrow"},
			{LCH::KeyCode::UpArrow, L"UpArrow"},
			{LCH::KeyCode::RightArrow, L"RightArrow"},
			{LCH::KeyCode::DownArrow, L"DownArrow"},
			{LCH::KeyCode::Insert, L"Insert"},
			{LCH::KeyCode::Delete, L"Delete"},
			{LCH::KeyCode::Num0, L"Num0"},
			{LCH::KeyCode::Num1, L"Num1"},
			{LCH::KeyCode::Num2, L"Num2"},
			{LCH::KeyCode::Num3, L"Num3"},
			{LCH::KeyCode::Num4, L"Num4"},
			{LCH::KeyCode::Num5, L"Num5"},
			{LCH::KeyCode::Num6, L"Num6"},
			{LCH::KeyCode::Num7, L"Num7"},
			{LCH::KeyCode::Num8, L"Num8"},
			{LCH::KeyCode::Num9, L"Num9"},
			{LCH::KeyCode::A, L"A"},
			{LCH::KeyCode::B, L"B"},
			{LCH::KeyCode::C, L"C"},
			{LCH::KeyCode::D, L"D"},
			{LCH::KeyCode::E, L"E"},
			{LCH::KeyCode::F, L"F"},
			{LCH::KeyCode::G, L"G"},
			{LCH::KeyCode::H, L"H"},
			{LCH::KeyCode::I, L"I"},
			{LCH::KeyCode::J, L"J"},
			{LCH::KeyCode::K, L"K"},
			{LCH::KeyCode::L, L"L"},
			{LCH::KeyCode::M, L"M"},
			{LCH::KeyCode::N, L"N"},
			{LCH::KeyCode::O, L"O"},
			{LCH::KeyCode::P, L"P"},
			{LCH::KeyCode::Q, L"Q"},
			{LCH::KeyCode::R, L"R"},
			{LCH::KeyCode::S, L"S"},
			{LCH::KeyCode::T, L"T"},
			{LCH::KeyCode::U, L"U"},
			{LCH::KeyCode::V, L"V"},
			{LCH::KeyCode::W, L"W"},
			{LCH::KeyCode::X, L"X"},
			{LCH::KeyCode::Y, L"Y"},
			{LCH::KeyCode::Z, L"Z"},
			{LCH::KeyCode::Keypad0, L"Keypad0"},
			{LCH::KeyCode::Keypad1, L"Keypad1"},
			{LCH::KeyCode::Keypad2, L"Keypad2"},
			{LCH::KeyCode::Keypad3, L"Keypad3"},
			{LCH::KeyCode::Keypad4, L"Keypad4"},
			{LCH::KeyCode::Keypad5, L"Keypad5"},
			{LCH::KeyCode::Keypad6, L"Keypad6"},
			{LCH::KeyCode::Keypad7, L"Keypad7"},
			{LCH::KeyCode::Keypad8, L"Keypad8"},
			{LCH::KeyCode::Keypad9, L"Keypad9"},
			{LCH::KeyCode::KeypadMutiply, L"KeypadMutiply"},
			{LCH::KeyCode::KeypadAdd, L"KeypadAdd"},
			{LCH::KeyCode::KeypadSubtract, L"KeypadSubtract"},
			{LCH::KeyCode::KeypadPeriod, L"KeypadPeriod"},
			{LCH::KeyCode::KeypadDivide, L"KeypadDivide"},
			{LCH::KeyCode::F1, L"F1"},
			{LCH::KeyCode::F2, L"F2"},
			{LCH::KeyCode::F3, L"F3"},
			{LCH::KeyCode::F4, L"F4"},
			{LCH::KeyCode::F5, L"F5"},
			{LCH::KeyCode::F6, L"F6"},
			{LCH::KeyCode::F7, L"F7"},
			{LCH::KeyCode::F8, L"F8"},
			{LCH::KeyCode::F9, L"F9"},
			{LCH::KeyCode::F10, L"F10"},
			{LCH::KeyCode::F11, L"F11"},
			{LCH::KeyCode::F12, L"F12"},
			{LCH::KeyCode::LeftShift, L"LeftShift"},
			{LCH::KeyCode::RightShift, L"RightShift"},
			{LCH::KeyCode::LeftControl, L"LeftControl"},
			{LCH::KeyCode::RightControl, L"RightControl"},
			{LCH::KeyCode::LeftAlt, L"LeftAlt"},
			{LCH::KeyCode::RightAlt, L"RightAlt"},
			{LCH::KeyCode::Semicolon, L"Semicolon"},
			{LCH::KeyCode::Comma, L"Comma"},
			{LCH::KeyCode::Add, L"Add"},
			{LCH::KeyCode::Subtract, L"Subtract"},
			{LCH::KeyCode::Period, L"Period"},
			{LCH::KeyCode::Slash, L"Slash"},
			{LCH::KeyCode::BackQuote, L"BackQuote"},
			{LCH::KeyCode::LeftBracket, L"LeftBracket"},
			{LCH::KeyCode::BackSlash, L"BackSlash"},
			{LCH::KeyCode::RightBracket, L"RightBracket"},
			{LCH::KeyCode::Quote, L"Quote"},
		};

		std::unordered_map<LCH::InputEvent::Type, wchar_t const*> types =
		{
			{LCH::InputEvent::Type::Press, L"Press"},
			{LCH::InputEvent::Type::Release, L"Release"},
			{LCH::InputEvent::Type::MouseMove, L"Mouse Move"},
			{LCH::InputEvent::Type::WheelRoll, L"Wheel Roll"},
		};
	};
}