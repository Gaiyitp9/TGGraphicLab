/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "WindowHelper.h"
#include <optional>

namespace TG
{
	// WIN32窗口基类
	class Window
	{
	public:
		Window(int x, int y, int width, int height, HWND parent);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;
		virtual ~Window() = default;

		static std::optional<int> ProcessMessage();							    // 处理所有窗口的消息

		[[nodiscard]] HWND Hwnd() const noexcept { return m_hwnd; }		        // 窗口句柄
		[[nodiscard]] HWND ParentHwnd() const noexcept { return m_parent; }     // 返回父母窗口
		[[nodiscard]] int Width() const noexcept { return m_width; }            // 获取窗口宽和高
		[[nodiscard]] int Height() const noexcept { return m_height; }
		[[nodiscard]] bool Destroy() const noexcept { return m_destroy; }       // 是否销毁窗口

        void SpyMessage(bool enable) noexcept { m_spyMessage = enable; }	    // 捕捉窗口消息

	private:
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) = 0;    // 消息处理函数

	protected:
		HWND m_hwnd;
		HWND m_parent;
		int m_posX, m_posY;
		int m_width, m_height;
		bool m_destroy = false;				// 是否销毁窗口
        bool m_spyMessage = false;			// 是否监控窗口消息
	};
}
