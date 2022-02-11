/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "CustomWindows.h"
#include "Window.h"
#include "WindowRegister.h"
#include "Debug.h"
#ifdef _DEBUG
#include <iostream>
#endif

namespace LCH
{
	Window::Window(int width, int height, int icon, const wchar_t* title)
		: width(width), height(height), icon(icon), hwnd(nullptr), input(nullptr)
	{
		// 初始化窗口，使用虚函数，不同类型的窗口可以使用自己的初始化窗口函数
		Initialize(width, height, title);
	}

	Window::~Window()
	{
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		std::wcout << WindowRegister::GetInstance()->GetWindowMesssageInfo(msg, wParam, lParam);

		switch (msg)
		{
		case WM_DESTROY:
			// 基础窗口一般作为主窗口，销毁后要退出线程
			PostQuitMessage(0);
			return 0;

			// 按下按键
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (input && !(lParam & 0x4000000))
			{
				input->OnKeyPressed(static_cast<size_t>(wParam));
			}
			break;

			// 松开按键
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (input)
				input->OnKeyReleased(static_cast<size_t>(wParam));
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	std::optional<int> Window::ProcessMessage()
	{
		MSG msg = { 0 };

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return std::nullopt;
	}

	int Window::GetIcon() const noexcept
	{
		return icon;
	}

	void Window::ConnectInputSystem(InputSystem* input)
	{
		this->input = input;
	}

	void Window::Initialize(int width, int height, const wchar_t* title)
	{
		// 获取窗口类名称
		WindowRegister* const windowRegister = WindowRegister::GetInstance();
		std::wstring wndClassName = windowRegister->GetWindowClassName(WindowType::Default);

		// 客户端区域大小
		RECT rect = { 0, 0, width, height };

		// 根据客户区域宽和高计算整个窗口的宽和高
		ASSERT(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false), true);
		// 创建窗口
		ASSERT(hwnd = CreateWindow(wndClassName.c_str(), title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, windowRegister->GetHInstance(), this), true);

		ShowWindow(hwnd, SW_SHOW);
	}
}
