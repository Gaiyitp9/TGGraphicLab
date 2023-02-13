/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/InputEvent.hpp"

std::unordered_map<TG::KeyCode, wchar_t const*> TG::InputEventInfo::keysName =
{
	{TG::KeyCode::None, L"None"},
	{TG::KeyCode::LeftMouseButton, L"LeftMouseButton"},
	{TG::KeyCode::RightMouseButton, L"RightMouseButton"},
	{TG::KeyCode::MidMouseButton, L"MidMouseButton"},
	{TG::KeyCode::Backspace, L"Backspace"},
	{TG::KeyCode::Tab, L"Tab"},
	{TG::KeyCode::Enter, L"Enter"},
	{TG::KeyCode::Esc, L"Esc"},
	{TG::KeyCode::Space, L"Space"},
	{TG::KeyCode::PageUp, L"PageUp"},
	{TG::KeyCode::PageDown, L"PageDown"},
	{TG::KeyCode::End, L"End"},
	{TG::KeyCode::Home, L"Home"},
	{TG::KeyCode::LeftArrow, L"LeftArrow"},
	{TG::KeyCode::UpArrow, L"UpArrow"},
	{TG::KeyCode::RightArrow, L"RightArrow"},
	{TG::KeyCode::DownArrow, L"DownArrow"},
	{TG::KeyCode::Insert, L"Insert"},
	{TG::KeyCode::Delete, L"Delete"},
	{TG::KeyCode::Num0, L"Num0"},
	{TG::KeyCode::Num1, L"Num1"},
	{TG::KeyCode::Num2, L"Num2"},
	{TG::KeyCode::Num3, L"Num3"},
	{TG::KeyCode::Num4, L"Num4"},
	{TG::KeyCode::Num5, L"Num5"},
	{TG::KeyCode::Num6, L"Num6"},
	{TG::KeyCode::Num7, L"Num7"},
	{TG::KeyCode::Num8, L"Num8"},
	{TG::KeyCode::Num9, L"Num9"},
	{TG::KeyCode::A, L"A"},
	{TG::KeyCode::B, L"B"},
	{TG::KeyCode::C, L"C"},
	{TG::KeyCode::D, L"D"},
	{TG::KeyCode::E, L"E"},
	{TG::KeyCode::F, L"F"},
	{TG::KeyCode::G, L"G"},
	{TG::KeyCode::H, L"H"},
	{TG::KeyCode::I, L"I"},
	{TG::KeyCode::J, L"J"},
	{TG::KeyCode::K, L"K"},
	{TG::KeyCode::L, L"L"},
	{TG::KeyCode::M, L"M"},
	{TG::KeyCode::N, L"N"},
	{TG::KeyCode::O, L"O"},
	{TG::KeyCode::P, L"P"},
	{TG::KeyCode::Q, L"Q"},
	{TG::KeyCode::R, L"R"},
	{TG::KeyCode::S, L"S"},
	{TG::KeyCode::T, L"T"},
	{TG::KeyCode::U, L"U"},
	{TG::KeyCode::V, L"V"},
	{TG::KeyCode::W, L"W"},
	{TG::KeyCode::X, L"X"},
	{TG::KeyCode::Y, L"Y"},
	{TG::KeyCode::Z, L"Z"},
	{TG::KeyCode::Keypad0, L"Keypad0"},
	{TG::KeyCode::Keypad1, L"Keypad1"},
	{TG::KeyCode::Keypad2, L"Keypad2"},
	{TG::KeyCode::Keypad3, L"Keypad3"},
	{TG::KeyCode::Keypad4, L"Keypad4"},
	{TG::KeyCode::Keypad5, L"Keypad5"},
	{TG::KeyCode::Keypad6, L"Keypad6"},
	{TG::KeyCode::Keypad7, L"Keypad7"},
	{TG::KeyCode::Keypad8, L"Keypad8"},
	{TG::KeyCode::Keypad9, L"Keypad9"},
	{TG::KeyCode::KeypadMutiply, L"KeypadMutiply"},
	{TG::KeyCode::KeypadAdd, L"KeypadAdd"},
	{TG::KeyCode::KeypadSubtract, L"KeypadSubtract"},
	{TG::KeyCode::KeypadPeriod, L"KeypadPeriod"},
	{TG::KeyCode::KeypadDivide, L"KeypadDivide"},
	{TG::KeyCode::F1, L"F1"},
	{TG::KeyCode::F2, L"F2"},
	{TG::KeyCode::F3, L"F3"},
	{TG::KeyCode::F4, L"F4"},
	{TG::KeyCode::F5, L"F5"},
	{TG::KeyCode::F6, L"F6"},
	{TG::KeyCode::F7, L"F7"},
	{TG::KeyCode::F8, L"F8"},
	{TG::KeyCode::F9, L"F9"},
	{TG::KeyCode::F10, L"F10"},
	{TG::KeyCode::F11, L"F11"},
	{TG::KeyCode::F12, L"F12"},
	{TG::KeyCode::LeftShift, L"LeftShift"},
	{TG::KeyCode::RightShift, L"RightShift"},
	{TG::KeyCode::LeftControl, L"LeftControl"},
	{TG::KeyCode::RightControl, L"RightControl"},
	{TG::KeyCode::LeftAlt, L"LeftAlt"},
	{TG::KeyCode::RightAlt, L"RightAlt"},
	{TG::KeyCode::Semicolon, L"Semicolon"},
	{TG::KeyCode::Comma, L"Comma"},
	{TG::KeyCode::Add, L"Add"},
	{TG::KeyCode::Subtract, L"Subtract"},
	{TG::KeyCode::Period, L"Period"},
	{TG::KeyCode::Slash, L"Slash"},
	{TG::KeyCode::BackQuote, L"BackQuote"},
	{TG::KeyCode::LeftBracket, L"LeftBracket"},
	{TG::KeyCode::BackSlash, L"BackSlash"},
	{TG::KeyCode::RightBracket, L"RightBracket"},
	{TG::KeyCode::Quote, L"Quote"},
};

std::unordered_map<TG::InputEvent::Type, wchar_t const*> TG::InputEventInfo::eventTypes =
{
	{TG::InputEvent::Type::Press, L"Press"},
	{TG::InputEvent::Type::Release, L"Release"},
	{TG::InputEvent::Type::MouseMove, L"Mouse Move"},
	{TG::InputEvent::Type::WheelRoll, L"Wheel Roll"},
};