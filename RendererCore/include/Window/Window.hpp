/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include "Window/WindowRegister.hpp"
#include "Diagnostics/Debug.hpp"
#include "Chronometer.hpp"
#include <optional>

namespace LCH
{
	// 窗口基类
	class Window
	{
	public:
		Window(int x, int y, int width, int height, std::shared_ptr<Window> parent);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window() = 0;

		static std::optional<int> ProcessMessage();							// 处理所有窗口的消息
		virtual void Update() = 0;											// 更新窗口

		const HWND Hwnd() const noexcept;									// 窗口句柄
		const std::weak_ptr<Window>& ParentWindow() const noexcept;			// 返回父母窗口
		int Width() const noexcept;											// 获取窗口宽和高
		int Height() const noexcept;
		bool Destroy() const noexcept;										// 是否销毁窗口

	protected:
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	// 消息处理函数

	protected:
		HWND hwnd;
		int posX, posY;
		int width, height;
		std::weak_ptr<Window> parent;
		bool destroy = false;				// 是否销毁窗口

		friend class WindowRegister;
	};
}