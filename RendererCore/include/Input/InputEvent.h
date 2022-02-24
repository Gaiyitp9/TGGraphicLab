/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
#include <format>
#include <array>
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


	template<typename CharT>
	struct std::formatter<InputEvent, CharT> : std::formatter<std::wstring, CharT>
	{
		template<typename FormatContext>
		auto format(InputEvent e, FormatContext& fc)
		{

			return std::formatter<std::wstring, CharT>::format();
		}

	private:
		std::array<wchar_t const*, 97> KeysName =
		{
			L"LeftMouseButton",
			L"RightMouseButton",
			L"MidMouseButton",
			L"Backspace",
			L"Tab",
			L"Esc",
			L"Space",
			L"PageUp",
			L"PageDown",
			L"End",
			L"Home",
			L"LeftArrow",
			L"UpArrow",
			L"RightArrow",
			L"DownArrow",
			L"Insert",
			L"Delete",
			L"Num0",
			L"Num1",
			L"Num2",
			L"Num3",
			L"Num4",
			L"Num5",
			L"Num6",
			L"Num7",
			L"Num8",
			L"Num9",
			L"A",
			L"B",
			L"C",
			L"D",
			L"E",
			L"F",
			L"G",
			L"H",
			L"I",
			L"J",
			L"K",
			L"L",
			L"M",
			L"N",
			L"O",
			L"P",
			L"Q",
			L"R",
			L"S",
			L"T",
			L"U",
			L"V",
			L"W",
			L"X",
			L"Y",
			L"Z",
			L"Keypad0",
			L"Keypad1",
			L"Keypad2",
			L"Keypad3",
			L"Keypad4",
			L"Keypad5",
			L"Keypad6",
			L"Keypad7",
			L"Keypad8",
			L"Keypad9",
			L"KeypadMutiply",
			L"KeypadAdd",
			L"KeypadSubtract",
			L"KeypadPeriod",
			L"KeypadDivide",
			L"F1",
			L"F2",
			L"F3",
			L"F4",
			L"F5",
			L"F6",
			L"F7",
			L"F8",
			L"F9",
			L"F10",
			L"F11",
			L"F12",
			L"LeftShift",
			L"RightShift",
			L"LeftControl",
			L"RightControl",
			L"LeftAlt",
			L"RightAlt",
			L"Semicolon",
			L"Comma",
			L"Add",
			L"Subtract",
			L"Period",
			L"Slash",
			L"BackQuote",
			L"LeftBracket",
			L"BackSlash",
			L"RightBracket",
			L"Quote",
		};

		std::array<wchar_t const*, 11> types =
		{
			L"Pressed",
			L"Released",

			L"LPress",
			L"LRelease",
			L"RPress",
			L"RRelease",
			L"WheelUp",
			L"WheelDown",
			L"Move",
			L"Enter",
			L"Leave"
		};
	};
}