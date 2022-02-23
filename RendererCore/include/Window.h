/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include "Input/InputSystem.h"
#include "Diagnostics/Debug.h"
#include <optional>

namespace LCH
{
	// 窗口类
	class Window
	{
	public:
		Window(int width, int height, wchar_t const* title = L"TG RenderLab", HWND parent = nullptr);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window();

		static std::optional<int> ProcessMessage();							// 处理所有窗口的消息

		const HWND GetHwnd() const noexcept;
		const HWND GetParentHwnd() const noexcept;
		const std::wstring& GetName() const noexcept;

		const InputSystem& Input() const noexcept;

	public:
		bool spyMessage = false;											// 是否监控窗口消息

	private:
		virtual void Initialize();											// 初始化窗口，子类窗口可以重写初始化窗口函数
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	// 消息处理函数，子类可以重写

	private:
		HWND hwnd, parentHwnd;
		int width, height;
		std::wstring name;

		InputSystem input;													// 输入系统，用于记录输入信息，比如键盘和鼠标

		static BaseException const* windowProcException;					// 用于记录窗口处理函数异常(因为异常不能从窗口处理函数向上传播)
		friend class WindowRegister;
	};
}