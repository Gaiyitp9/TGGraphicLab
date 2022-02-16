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
		Window(int width, int height, std::optional<int> icon = std::nullopt, const wchar_t* title = L"TG RenderLab");
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();
		static std::optional<int> ProcessMessage();	// 处理窗口消息

	public:
		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	// 消息处理函数，子类可以重写
		std::optional<int> GetIcon() const noexcept;

	protected:
		// 初始化窗口，子类窗口可以重写初始化窗口函数
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
		std::optional<int> icon; // 窗口icon索引
	};
}