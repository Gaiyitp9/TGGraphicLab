/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window.h"
#include "WindowRegister.h"

namespace LCH
{
	Window::Window(int width, int height, wchar_t const* title, HWND parent)
		: width(width), height(height), hwnd(nullptr), name(title), parentHwnd(parent)
	{
		Initialize();
	}

	Window::~Window()
	{
	}

	void Window::Update()
	{
		input.Update();
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			// 基础窗口一般作为主窗口，销毁后要退出线程
			PostQuitMessage(0);
			return 0;

			// 按下按键
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			// 是否记录重复按键
			if (!(lParam & 0x40000000) || input.keyboard.autoRepeat)
			{
				WPARAM keyCode = input.keyboard.MapLeftRightKey(wParam, lParam);
				input.keyboard.OnKeyPressed(static_cast<unsigned char>(keyCode));
			}
			return 0;

			// 松开按键
		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				WPARAM keyCode = input.keyboard.MapLeftRightKey(wParam, lParam);
				input.keyboard.OnKeyReleased(static_cast<unsigned char>(keyCode));
				return 0;
			}

			// 按键字符
		case WM_CHAR:
			if (!(lParam & 0x40000000) || input.keyboard.autoRepeat)
			{
				input.keyboard.OnChar(static_cast<char>(wParam));
			}
			return 0;

			// 鼠标移动
		case WM_MOUSEMOVE:
			input.mouse.OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
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

	const std::wstring& Window::GetName() const noexcept
	{
		return name;
	}

	const HWND Window::GetHwnd() const noexcept
	{
		return hwnd;
	}

	const HWND Window::GetParentHwnd() const noexcept
	{
		return parentHwnd;
	}

	bool Window::Exist() const noexcept
	{
		return exist;
	}

	const InputSystem& Window::Input() const noexcept
	{
		return input;
	}

	void Window::Initialize()
	{
		// 获取窗口类名称
		WindowRegister& windowRegister = WindowRegister::GetInstance();
		const std::wstring& wndClassName = windowRegister.GetWindowClassName(WindowType::Default);

		// 客户端区域大小
		RECT rect = { 0, 0, width, height };

		// 根据客户区域宽和高计算整个窗口的宽和高
		if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
			ThrowLastError();

		// 创建窗口
		hwnd = CreateWindowW(wndClassName.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
			parentHwnd, nullptr, windowRegister.GetHInstance(), this);

		if (hwnd == nullptr)
			ThrowLastError();

		ShowWindow(hwnd, SW_SHOW);
	}
}
