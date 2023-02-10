/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window/Window.hpp"

namespace TG
{
	Window::Window(int x, int y, int width, int height, std::shared_ptr<Window> parent)
		: posX(x), posY(y), width(width), height(height), hwnd(nullptr), parent(parent)
	{
	}

	Window::~Window()
	{

	}

	std::optional<int> Window::ProcessMessage()
	{
		MSG msg = { 0 };

		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return std::nullopt;
	}

	const HWND Window::Hwnd() const noexcept
	{
		return hwnd;
	}

	const std::weak_ptr<Window>& Window::ParentWindow() const noexcept
	{
		return parent;
	}

	int Window::Width() const noexcept
	{
		return width;
	}

	int Window::Height() const noexcept
	{
		return height;
	}

	bool Window::Destroy() const noexcept
	{
		return destroy;
	}

	LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
