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

	void Keyboard::OnKeyPressed(unsigned char keyCode)
	{
		keyStates[keyCode] = true;
		eventBuffer.push(InputEvent{ static_cast<KeyCode>(keyCode), InputEvent::Type::Pressed });
		// 移除旧的按键事件
		while (eventBuffer.size() > BUFSIZE)
		{
			eventBuffer.pop();
		}
	}

	void Keyboard::OnKeyReleased(unsigned char keyCode)
	{
		keyStates[keyCode] = false;
		eventBuffer.push(InputEvent{ static_cast<KeyCode>(keyCode), InputEvent::Type::Released });
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