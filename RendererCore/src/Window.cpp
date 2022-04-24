/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window.h"
#include "WindowRegister.h"

namespace LCH
{
	Window::Window(int width, int height, wchar_t const* title, Window const* parent)
		: width(width), height(height), hwnd(nullptr), name(title), parentWnd(parent)
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
		{
			// 是否记录重复按键
			WPARAM keyCode = input.keyboard.MapLeftRightKey(wParam, lParam);
			input.keyboard.OnKeyPress(static_cast<KeyCode>(keyCode));
			return 0;
		}

			// 松开按键
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			WPARAM keyCode = input.keyboard.MapLeftRightKey(wParam, lParam);
			input.keyboard.OnKeyRelease(static_cast<KeyCode>(keyCode));
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
			input.mouse.OnMouseMove(MAKEPOINTS(lParam));
			return 0;

			// 按下鼠标左键
		case WM_LBUTTONDOWN:
			input.mouse.OnButtonPress(KeyCode::LeftMouseButton);
			return 0;

			// 松开鼠标左键
		case WM_LBUTTONUP:
			input.mouse.OnButtonRelease(KeyCode::LeftMouseButton);
			return 0;

			// 按下鼠标右键
		case WM_RBUTTONDOWN:
			input.mouse.OnButtonPress(KeyCode::RightMouseButton);
			return 0;

			// 松开鼠标右键
		case WM_RBUTTONUP:
			input.mouse.OnButtonRelease(KeyCode::RightMouseButton);
			return 0;

			// 按下鼠标中键
		case WM_MBUTTONDOWN:
			input.mouse.OnButtonPress(KeyCode::MidMouseButton);
			return 0;

			// 松开鼠标中键
		case WM_MBUTTONUP:
			input.mouse.OnButtonRelease(KeyCode::MidMouseButton);
			return 0;

			// 滚动鼠标滚轮
		case WM_MOUSEWHEEL:
			// 每帧只会产生一个WM_MOUSEWHEEL
			input.mouse.OnWheelRoll(KeyCode::MidMouseButton, GET_WHEEL_DELTA_WPARAM(wParam));
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

	void Window::SetIcon(int iconSource)
	{
		HICON icon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(iconSource));
		if (icon == nullptr)
			ThrowLastErrorWithDesc(L"Invalid icon source");

		if (SetClassLongPtrW(hwnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(icon)) == 0)
			ThrowLastError();
		if (SetClassLongPtrW(hwnd, GCLP_HICONSM, reinterpret_cast<LONG_PTR>(icon)) == 0)
			ThrowLastError();
	}

	const std::wstring& Window::Name() const noexcept
	{
		return name;
	}

	bool Window::Destroy() const noexcept
	{
		return destroy;
	}

	const HWND Window::Hwnd() const noexcept
	{
		return hwnd;
	}

	Window const* Window::ParentWindow() const noexcept
	{
		return parentWnd;
	}

	const InputSystem& Window::Input() const noexcept
	{
		return input;
	}

	void Window::SpyMessage(bool enable) noexcept
	{
		spyMessage = enable;
	}

	void Window::SpyInputEvent(bool enable) noexcept
	{
		input.keyboard.spyKeyboard = enable;
		input.mouse.spyMouse = enable;
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

		HWND parentHwnd = nullptr;
		if (parentWnd != nullptr)
			parentHwnd = parentWnd->hwnd;
		// 创建窗口
		hwnd = CreateWindowW(wndClassName.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
			200, 100, rect.right - rect.left, rect.bottom - rect.top,
			parentHwnd, nullptr, windowRegister.GetHInstance(), this);

		if (hwnd == nullptr)
			ThrowLastError();

		ShowWindow(hwnd, SW_SHOW);
	}
}
