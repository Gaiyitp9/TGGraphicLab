/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Keyboard.h"
#include <iostream>

namespace LCH
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
			eventBuffer.pop();
		}
	}

	void Keyboard::OnKeyPressed(unsigned char keyCode)
	{
		// 按键记录到队列中
		eventBuffer.push(InputEvent{ static_cast<KeyCode>(keyCode), InputEvent::Type::Press });
		// 移除旧的按键事件
		while (eventBuffer.size() > BUFSIZE)
		{
			eventBuffer.pop();
		}
	}

	void Keyboard::OnKeyReleased(unsigned char keyCode)
	{
		// 按键记录到队列中
		eventBuffer.push(InputEvent{ static_cast<KeyCode>(keyCode), InputEvent::Type::Release });
		// 移除旧的按键事件
		while (eventBuffer.size() > BUFSIZE)
		{
			eventBuffer.pop();
		}
	}

	void Keyboard::OnChar(char ch)
	{
		charBuffer.push(ch);
		// 移除旧的字符
		while (charBuffer.size() > BUFSIZE)
		{
			charBuffer.pop();
		}
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
}