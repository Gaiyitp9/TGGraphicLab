/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
#include <format>
#include <unordered_map>
#include <string>

namespace LCH
{
	struct InputEvent
	{
		enum class Type
		{
			Pressed,
			Released,

			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave
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
			return pc.end();
		}

		template<typename FormatContext>
		auto format(const LCH::InputEvent& e, FormatContext& fc)
		{
			return std::format_to(fc.out(), L"Key: {} Event: {}", keysName[e.key], types[e.type]);
		}

	private:
		std::unordered_map<LCH::KeyCode, wchar_t const*> keysName =
		{
			{LCH::KeyCode::LeftMouseButton, L"LeftMouseButton"},
			{LCH::KeyCode::RightMouseButton, L"RightMouseButton"},
			{LCH::KeyCode::MidMouseButton, L"MidMouseButton"},
			{LCH::KeyCode::Backspace, L"Backspace"},
			{LCH::KeyCode::Tab, L"Tab"},
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
			{LCH::InputEvent::Type::Pressed, L"Pressed"},
			{LCH::InputEvent::Type::Released, L"Released"},

			{LCH::InputEvent::Type::LPress, L"LPress"},
			{LCH::InputEvent::Type::LRelease, L"LRelease"},
			{LCH::InputEvent::Type::RPress, L"RPress"},
			{LCH::InputEvent::Type::RRelease, L"RRelease"},
			{LCH::InputEvent::Type::WheelUp, L"WheelUp"},
			{LCH::InputEvent::Type::WheelDown, L"WheelDown"},
			{LCH::InputEvent::Type::Move, L"Move"},
			{LCH::InputEvent::Type::Enter, L"Enter"},
			{LCH::InputEvent::Type::Leave, L"Leave"},
		};
	};
}