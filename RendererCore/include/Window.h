/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "SlimWindows.h"
#include "InputSystem.h"
#include <optional>

namespace LCH
{
	// 窗口类
	class Window
	{
	public:
		Window(int width, int height, const wchar_t* title = L"TG RenderLab", HWND parent = nullptr);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window();

		static std::optional<int> ProcessMessage();							// 处理所有窗口的消息

		const HWND GetHwnd() const noexcept;
		const HWND GetParentHwnd() const noexcept;
		const std::wstring& GetName() const noexcept;

	private:
		virtual void Initialize();											// 初始化窗口，子类窗口可以重写初始化窗口函数
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	// 消息处理函数，子类可以重写

	private:
		HWND hwnd, parentHwnd;
		int width, height;
		std::wstring name;

		friend class WindowRegister;
	};
}