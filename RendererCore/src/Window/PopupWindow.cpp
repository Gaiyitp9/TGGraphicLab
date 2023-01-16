/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window/PopupWindow.hpp"

namespace TG
{
	PopupWindow::PopupWindow(int x, int y, int width, int height, std::shared_ptr<Window> parent)
		: Window(x, y, width, height, parent)
	{
		Initialize();
	}

	PopupWindow::~PopupWindow()
	{

	}

	void PopupWindow::Update()
	{

	}

	void PopupWindow::Initialize()
	{
		// 获取窗口类名称
		WindowRegister& windowRegister = WindowRegister::instance();
		const std::wstring& wndClassName = windowRegister.GetWindowClassName(WindowType::Default);

		// 客户端区域大小
		RECT rect = { 0, 0, width, height };
		// 根据客户区域宽和高计算整个窗口的宽和高
		if (!AdjustWindowRect(&rect, WS_POPUP, false))
			ThrowLastError();

		HWND parentHwnd = nullptr;
		if (auto observe = parent.lock())
			parentHwnd = observe->Hwnd();
		// 创建窗口
		hwnd = CreateWindowW(wndClassName.c_str(), L"Popup", WS_POPUP,
			posX, posY, rect.right - rect.left, rect.bottom - rect.top,
			parentHwnd, nullptr, windowRegister.hInstance(), this);

		if (hwnd == nullptr)
			ThrowLastError();

		ShowWindow(hwnd, SW_SHOW);

	}

	LRESULT CALLBACK PopupWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Window::WindowProc(hwnd, msg, wParam, lParam);
	}
}