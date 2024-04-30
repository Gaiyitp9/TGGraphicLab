/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/Editor/PopupWindow.h"
#include "../../../../Include/PAL/Windows/WinAPIException.h"
#include "Diagnostics/Log.hpp"
#include <format>

namespace TG::PAL
{
	PopupWindow::PopupWindow(int x, int y, int width, int height, HWND parent)
		: Window(x, y, width, height, parent)
	{
        // 客户端区域大小
        RECT rect = { 0, 0, m_width, m_height };
        // 根据客户区域宽和高计算整个窗口的宽和高
        if (!AdjustWindowRect(&rect, WS_POPUP, false))
            CheckLastError();

        // 创建窗口
        m_hwnd = CreateWindowW(GetWindowClassName(WindowCategory::Default), L"Popup", WS_POPUP,
                               m_posX, m_posY, rect.right - rect.left, rect.bottom - rect.top,
                               m_parent, nullptr, nullptr, this);

        if (m_hwnd == nullptr)
            CheckLastError();

        ShowWindow(m_hwnd, SW_SHOW);
	}

	PopupWindow::~PopupWindow() = default;

	LRESULT CALLBACK PopupWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// 是否监控窗口消息
		if (m_spyMessage)
			Debug::LogLine(std::format("{:<16} {}", "Popup", GetWindowMessageInfo(msg, wParam, lParam)));

        switch (msg)
        {
            case WM_DESTROY:
                m_destroy = true;
                return 0;

        	default:
				return DefWindowProcW(hwnd, msg, wParam, lParam);
        }
	}
}
