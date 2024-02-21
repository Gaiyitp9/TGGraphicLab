/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/Window.h"

namespace TG
{
	Window::Window(int x, int y, int width, int height, HWND parent)
		: m_posX(x), m_posY(y), m_width(width), m_height(height), m_hwnd(nullptr), m_parent(parent)
	{}

	std::optional<int> Window::ProcessMessage()
	{
		MSG msg = { nullptr };

		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return std::nullopt;
	}
}
