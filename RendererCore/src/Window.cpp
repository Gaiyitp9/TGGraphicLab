/****************************************************************
* Copyright (c) IGOTGAMES(IGG ShenZhen). All rights reserved.	*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/

#include "Window.h"

namespace IGGSZLab
{
	WindowClassRegister* WindowClassRegister::instance = nullptr;
	std::mutex WindowClassRegister::mutex;

	WindowClassRegister::WindowClassRegister()
	{
		hInstance = GetModuleHandle(nullptr);

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProcSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), 
			IMAGE_ICON, 32, 32, 0));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.hIconSm = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"Default";

		RegisterClassEx(&wc);
		windowClassMap[WindowType::Default] =  L"Default";
	}

	WindowClassRegister::~WindowClassRegister()
	{
		for (auto& pair : windowClassMap)
			UnregisterClass(pair.second.c_str(), hInstance);
	}

	LRESULT WindowClassRegister::WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcThunk));
			return pWnd->WindowProc(hwnd, msg, wParam, lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT WindowClassRegister::WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWnd->WindowProc(hwnd, msg, wParam, lParam);
	}

	WindowClassRegister* const WindowClassRegister::GetInstance()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr)
			instance = new WindowClassRegister();
		return instance;
	}

	Window::Window(int width, int height, const wchar_t* title = L"IGGSZ Lab", WindowType type = WindowType::Default)
		: width(width), height(height), hwnd(nullptr)
	{
		// 获取窗口类名称
		WindowClassRegister* const windowRegister = WindowClassRegister::GetInstance();
		std::wstring wndClassName = windowRegister->windowClassMap[type];

		// 根据客户区域宽和高计算整个窗口的宽和高
		RECT rect;
		rect.left = 0;
		rect.right = rect.left + width;
		rect.top = 0;
		rect.bottom = rect.top + height;

		switch (type)
		{
		case WindowType::Default:
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
			hwnd = CreateWindow(wndClassName.c_str(), title, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 
				nullptr, nullptr, windowRegister->hInstance, this);
			break;
		}

		ShowWindow(hwnd, SW_SHOWDEFAULT);
	}

	Window::~Window()
	{
		DestroyWindow(hwnd);
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
