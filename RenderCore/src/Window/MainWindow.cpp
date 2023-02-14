/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window/MainWindow.hpp"

namespace TG
{
	MainWindow::MainWindow(int x, int y, int width, int height, wchar_t const* name, std::shared_ptr<Window> parent)
		: Window(x, y, width, height, parent), name(name)
	{
		Initialize();
	}

	MainWindow::~MainWindow()
	{

	}

	void MainWindow::Update()
	{
		input.Update();
	}

	void MainWindow::SetIcon(int iconSource)
	{
		HICON icon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(iconSource));
		if (icon == nullptr)
			CheckLastError(L"Invalid icon source");

		SendMessageW(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessageW(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}

	const std::wstring& MainWindow::Name() const noexcept
	{
		return name;
	}

	const InputSystem& MainWindow::Input() const noexcept
	{
		return input;
	}

	void MainWindow::SetTimer(const std::shared_ptr<Chronometer>& timer) noexcept
	{
		this->timer = timer;
	}

	void MainWindow::SpyMessage(bool enable) noexcept
	{
		spyMessage = enable;
	}

	void MainWindow::SpyInputEvent(bool enable) noexcept
	{
		input.keyboard.spyKeyboard = enable;
		input.mouse.spyMouse = enable;
	}

	void MainWindow::Initialize()
	{
		// 获取窗口类名称
		WindowRegister& windowRegister = WindowRegister::instance();
		const std::wstring& wndClassName = windowRegister.GetWindowClassName(WindowType::Default);

		// 客户端区域大小
		RECT rect = { 0, 0, width, height };
		// 根据客户区域宽和高计算整个窗口的宽和高
		if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
			CheckLastError();

		HWND parentHwnd = nullptr;
		if (auto observer = parent.lock())
			parentHwnd = observer->Hwnd();
		// 创建窗口
		hwnd = CreateWindowW(wndClassName.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
			posX, posY, rect.right - rect.left, rect.bottom - rect.top,
			parentHwnd, nullptr, windowRegister.hInstance(), this);

		if (hwnd == nullptr)
			CheckLastError();

		ShowWindow(hwnd, SW_SHOW);
	}

	LRESULT MainWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// 是否监控窗口消息
		if (spyMessage)
			Debug::LogLine(WindowRegister::instance().GetWindowMesssageInfo(name, msg, wParam, lParam));

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
		{
			input.mouse.OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}

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

		case WM_SIZE:
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			if (auto observer = timer.lock())
			{
				if (wParam == SIZE_MINIMIZED)
					observer->Pause();
				else if (wParam == SIZE_RESTORED)
					observer->Start();
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			if (auto observer = timer.lock())
				observer->Pause();
			return 0;

		case WM_EXITSIZEMOVE:
			if (auto observer = timer.lock())
				observer->Start();
			return 0;
		}

		return Window::WindowProc(hwnd, msg, wParam, lParam);
	}
}