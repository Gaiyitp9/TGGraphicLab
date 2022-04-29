/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include "Diagnostics/Debug.h"
#include "Diagnostics/MemoryDbg.h"
#include "Input/InputSystem.h"
#include <optional>

namespace LCH
{
	// 窗口类，封装windows原生窗口
	class Window
	{
	public:
		Window(int width, int height, wchar_t const* title = L"TG RenderLab", Window const* parent = nullptr);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window();

		static std::optional<int> ProcessMessage();		// 处理所有窗口的消息
		void Update();									// 更新窗口

		void SetIcon(int icon);							// 设置窗口的icon

		const HWND Hwnd() const noexcept;				// 窗口句柄
		const std::wstring& Name() const noexcept;		// 窗口名称
		Window const* ParentWindow() const noexcept;	// 返回父母窗口
		const InputSystem& Input() const noexcept;		// 获取输入系统

		bool Destroy() const noexcept;					// 是否销毁窗口
		void SpyMessage(bool enable) noexcept;			// 捕捉窗口消息
		void SpyInputEvent(bool enable) noexcept;		// 是否监控输入事件

	private:
		virtual void Initialize();											// 初始化窗口，子类窗口可以重写初始化窗口函数
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	// 消息处理函数，子类可以重写

	private:
		HWND hwnd; 
		int width, height;
		std::wstring name;
		Window const* parentWnd = nullptr;

		InputSystem input;				// 输入系统，用于记录输入信息，比如键盘和鼠标

		bool destroy = false;			// 是否销毁窗口
		bool spyMessage = false;		// 是否监控窗口消息

		friend class WindowRegister;
	};
}