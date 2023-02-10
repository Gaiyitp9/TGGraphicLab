/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Keyboard.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG
{
	Keyboard::Keyboard()
	{

	}

	Keyboard::~Keyboard()
	{

	}

	void Keyboard::Update()
	{
		keyDown.reset();
		keyUp.reset();
		while (!eventBuffer.empty())
		{
			InputEvent e = eventBuffer.front();
			eventBuffer.pop();

			// 监控键盘
			if (spyKeyboard)
				Debug::LogLine(std::format(L"{}", e));

			size_t key = static_cast<size_t>(e.key);
			switch (e.type)
			{
			case InputEvent::Type::Press:
				// 第一次按下，key down为true
				if (!keyStates.test(key))
					keyDown[key] = true;
				keyStates[key] = true;
				break;

			case InputEvent::Type::Release:
				// 放开按键，key up为true(不存在第一次放开，每一次WM_KEYUP都是第一次放开)
				keyUp[key] = true;
				keyStates[key] = false;
				break;
			}
		}

		if (spyKeyboard)
		{
			while (!charBuffer.empty())
			{
				std::string c;
				c.append(1, charBuffer.front());
				Debug::LogLine(c);
				charBuffer.pop();
			}
		}
	}

	void Keyboard::OnKeyPress(KeyCode keyCode)
	{
		// 按键记录到队列中
		eventBuffer.push(InputEvent{ keyCode, InputEvent::Type::Press });
		TrimEventBuffer();
	}

	void Keyboard::OnKeyRelease(KeyCode keyCode)
	{
		// 按键记录到队列中
		eventBuffer.push(InputEvent{ keyCode, InputEvent::Type::Release });
		TrimEventBuffer();
	}

	void Keyboard::OnChar(char ch)
	{
		charBuffer.push(ch);
		TrimCharBuffer();
	}

	WPARAM Keyboard::MapLeftRightKey(WPARAM wParam, LPARAM lParam)
	{
		WPARAM mappedVK;
		UINT scancode = (lParam & 0x00ff0000) >> 16;
		int extended = (lParam & 0x01000000) != 0;

		switch (wParam) 
		{
		case VK_SHIFT:
			mappedVK = MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CONTROL:
			mappedVK = extended ? VK_RCONTROL : VK_LCONTROL;
			break;
		case VK_MENU:
			mappedVK = extended ? VK_RMENU : VK_LMENU;
			break;
		default:
			// 其他按键不需要重新映射
			mappedVK = wParam;
			break;
		}

		return mappedVK;
	}

	bool Keyboard::IsKeyboardCode(KeyCode code)
	{
		unsigned char index = static_cast<unsigned char>(code);
		if (index > 0x08 && index <= 0xDE)
			return true;
		return false;
	}

	void Keyboard::TrimEventBuffer()
	{
		// 移除旧的按键事件
		while (eventBuffer.size() > BUFSIZE)
			eventBuffer.pop();
	}

	void Keyboard::TrimCharBuffer()
	{
		// 移除旧的字符
		while (charBuffer.size() > BUFSIZE)
			charBuffer.pop();
	}
}