/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window/MainWindow.hpp"

namespace TG
{
	MainWindow::MainWindow(int x, int y, int width, int height, wchar_t const* name, HWND parent)
		: Window(x, y, width, height, parent), m_name(name)
	{
        // 获取窗口类名称
        WindowRegister& windowRegister = WindowRegister::Instance();

        // 客户端区域大小
        RECT rect = { 0, 0, m_width, m_height };
        // 根据客户区域宽和高计算整个窗口的宽和高
        if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
            CheckLastError();

        // 创建窗口
        m_hwnd = CreateWindowW(L"Default", m_name.c_str(), WS_OVERLAPPEDWINDOW,
                               m_posX, m_posY, rect.right - rect.left, rect.bottom - rect.top,
                               m_parent, nullptr, windowRegister.HInstance(), this);

        if (m_hwnd == nullptr)
            CheckLastError();

        ShowWindow(m_hwnd, SW_SHOW);
	}

	MainWindow::~MainWindow() = default;

    void MainWindow::SetInput(std::function<void(const Input::Event&)>&& send)
    {
        m_send = send;
    }

    void MainWindow::SetTimer(std::function<void()>&& start, std::function<void()>&& pause)
    {
        m_start = start;
        m_pause = pause;
    }

	void MainWindow::SetIcon(wchar_t const* iconPath)
	{
        HANDLE icon = LoadImageW(nullptr, iconPath, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		if (icon == nullptr)
			CheckLastError(L"Invalid icon source");

		SendMessageW(m_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessageW(m_hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}

	LRESULT MainWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// 是否监控窗口消息
		if (m_spyMessage)
			Debug::LogLine(WindowRegister::Instance().GetWindowMesssageInfo(m_name, msg, wParam, lParam));

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
            if (m_send)
            {
                WPARAM keyCode = Utility::MapLeftRightKey(wParam, lParam);
                m_send({Input::DeviceType::Keyboard, static_cast<Input::KeyCode>(keyCode), Input::EventType::Press, {}});
            }
			return 0;
		}

		// 松开按键
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
            if (m_send)
            {
                WPARAM keyCode = Utility::MapLeftRightKey(wParam, lParam);
                m_send({Input::DeviceType::Keyboard, static_cast<Input::KeyCode>(keyCode), Input::EventType::Release, {}});
            }
			return 0;
		}

		// 按键字符
		case WM_CHAR:
        {
//			if (!(lParam & 0x40000000) || m_input.m_keyboard.m_autoRepeat)
//			{
//				m_input.m_keyboard.OnChar(static_cast<char>(wParam));
//			}
            if (m_send)
            {
                Input::KeyboardData data{static_cast<char>(wParam)};
                m_send({Input::DeviceType::Keyboard, Input::KeyCode::None, Input::EventType::Char, data});
            }
			return 0;
        }

		// 鼠标移动
		case WM_MOUSEMOVE:
		{
            if (m_send)
            {
                Input::MouseData data{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                m_send({Input::DeviceType::Mouse, Input::KeyCode::None, Input::EventType::MouseMove, data});
            }
			return 0;
		}

		// 按下鼠标左键
		case WM_LBUTTONDOWN:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::LeftMouseButton, Input::EventType::Press, {}});
			return 0;

		// 松开鼠标左键
		case WM_LBUTTONUP:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::LeftMouseButton, Input::EventType::Release, {}});
			return 0;

		// 按下鼠标右键
		case WM_RBUTTONDOWN:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::RightMouseButton, Input::EventType::Press, {}});
			return 0;

		// 松开鼠标右键
		case WM_RBUTTONUP:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::RightMouseButton, Input::EventType::Release, {}});
			return 0;

		// 按下鼠标中键
		case WM_MBUTTONDOWN:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::MidMouseButton, Input::EventType::Press, {}});
			return 0;

		// 松开鼠标中键
		case WM_MBUTTONUP:
            if (m_send)
                m_send({Input::DeviceType::Mouse, Input::KeyCode::MidMouseButton, Input::EventType::Release, {}});
			return 0;

		// 滚动鼠标滚轮
		case WM_MOUSEWHEEL:
        {
            if (m_send)
            {
                // 每帧只会产生一个WM_MOUSEWHEEL
                Input::MouseData data{GET_WHEEL_DELTA_WPARAM(wParam)};
                m_send({Input::DeviceType::Mouse, Input::KeyCode::MidMouseButton, Input::EventType::WheelRoll, data});
            }
			return 0;
        }

		case WM_SIZE:
            if (m_pause && m_start)
            {
                m_width = LOWORD(lParam);
                m_height = HIWORD(lParam);
                if (wParam == SIZE_MINIMIZED)
                    m_pause();
                else if (wParam == SIZE_RESTORED)
                    m_start();
            }
			return 0;

		case WM_ENTERSIZEMOVE:
            if (m_pause)
			    m_pause();
			return 0;

		case WM_EXITSIZEMOVE:
            if (m_start)
			    m_start();
			return 0;

        default:
		    return Window::WindowProc(hwnd, msg, wParam, lParam);
		}
	}
}