/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/MainWindow.h"
#include "Diagnostics/Win32Exception.h"
#include "Diagnostics/Log.hpp"
#include "Input/EventData.h"
#include "Utility.h"
#include <format>

namespace TG
{
	MainWindow::MainWindow(int x, int y, int width, int height, wchar_t const* name, HWND parent)
		: Window(x, y, width, height, parent), m_name(name)
	{
        // 客户端区域大小
        RECT rect = { 0, 0, m_width, m_height };
        // 根据客户区域宽和高计算整个窗口的宽和高
        if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
            CheckLastError();

        // 创建窗口
        m_hwnd = CreateWindowW(GetWindowClassName(WindowType::Default), m_name.c_str(), WS_OVERLAPPEDWINDOW,
                               m_posX, m_posY, rect.right - rect.left, rect.bottom - rect.top,
                               m_parent, nullptr, nullptr, this);

        if (m_hwnd == nullptr)
            CheckLastError();

        ShowWindow(m_hwnd, SW_SHOW);
	}

	MainWindow::~MainWindow() = default;

    void MainWindow::SetInputListener(const std::function<void(const Input::Event &)>& listener)
    {
        m_listener = listener;
    }

    void MainWindow::SetStateCallback(const std::function<void()>& resume, const std::function<void()>& suspend)
    {
        m_resume = resume;
        m_suspend = suspend;
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
			Debug::LogLine(std::format("{:<16} {}", Utility::Utf16ToUtf8(m_name), GetWindowMessageInfo(msg, wParam, lParam)));

		switch (msg)
		{
		case WM_DESTROY:
            // 窗口被销毁后，窗口类也需要被销毁
            m_destroy = true;
			// 基础窗口一般作为主窗口，销毁后要退出线程
			PostQuitMessage(0);
			return 0;

		// 按下按键
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
		{
            if (m_listener)
            {
                // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input
                WORD vkCode = LOWORD(wParam);
                WORD keyFlags = HIWORD(lParam);
                WORD scanCode = LOBYTE(keyFlags);
                BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix
                if (isExtendedKey)
                    scanCode = MAKEWORD(scanCode, 0xE0);
                switch (vkCode)
                {
                    case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
                    case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
                    case VK_MENU:    // converts to VK_LMENU or VK_RMENU
                        vkCode = LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
                        break;

                    default:
                        break;
                }
                Input::EventType type = (keyFlags & KF_UP) == KF_UP ? Input::EventType::Release : Input::EventType::Press;
                m_listener({static_cast<Input::KeyCode>(vkCode), type, nullptr});
            }
			return 0;
		}

		// 按键字符
		case WM_CHAR:
        {
            if (m_listener)
            {
                Input::KeyboardData data{static_cast<char>(wParam)};
                m_listener({Input::KeyCode::Char, Input::EventType::Char, data});
            }
			return 0;
        }

		// 鼠标移动
		case WM_MOUSEMOVE:
		{
            if (m_listener)
            {
                Input::MouseData data{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                m_listener({Input::KeyCode::None, Input::EventType::MouseMove, data});
            }
			return 0;
		}

		// 按下鼠标左键
		case WM_LBUTTONDOWN:
            if (m_listener)
                m_listener({Input::KeyCode::LeftMouseButton, Input::EventType::Press, nullptr});
			return 0;

		// 松开鼠标左键
		case WM_LBUTTONUP:
            if (m_listener)
                m_listener({Input::KeyCode::LeftMouseButton, Input::EventType::Release, nullptr});
			return 0;

		// 按下鼠标右键
		case WM_RBUTTONDOWN:
            if (m_listener)
                m_listener({Input::KeyCode::RightMouseButton, Input::EventType::Press, nullptr});
			return 0;

		// 松开鼠标右键
		case WM_RBUTTONUP:
            if (m_listener)
                m_listener({Input::KeyCode::RightMouseButton, Input::EventType::Release, nullptr});
			return 0;

		// 按下鼠标中键
		case WM_MBUTTONDOWN:
            if (m_listener)
                m_listener({Input::KeyCode::MidMouseButton, Input::EventType::Press, nullptr});
			return 0;

		// 松开鼠标中键
		case WM_MBUTTONUP:
            if (m_listener)
                m_listener({Input::KeyCode::MidMouseButton, Input::EventType::Release, nullptr});
			return 0;

		// 滚动鼠标滚轮
		case WM_MOUSEWHEEL:
        {
            if (m_listener)
            {
                // 每帧只会产生一个WM_MOUSEWHEEL
                Input::MouseData data{GET_WHEEL_DELTA_WPARAM(wParam)};
                m_listener({Input::KeyCode::MidMouseButton, Input::EventType::WheelRoll, data});
            }
			return 0;
        }

		case WM_SIZE:
            if (m_resume && m_suspend)
            {
                m_width = LOWORD(lParam);
                m_height = HIWORD(lParam);
                if (wParam == SIZE_MINIMIZED)
                    m_suspend();
                else if (wParam == SIZE_RESTORED)
                    m_resume();
            }
			return 0;

		case WM_ENTERSIZEMOVE:
            if (m_suspend)
                m_suspend();
			return 0;

		case WM_EXITSIZEMOVE:
            if (m_resume)
                m_resume();
			return 0;

        default:
		    return DefWindowProcW(hwnd, msg, wParam, lParam);
		}
	}
}