/****************************************************************
* Copyright (c) IGOTGAMES(IGG ShenZhen). All rights reserved.	*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/

#pragma once

#include "pch.h"

namespace IGGSZLab
{
	// 窗口种类
	enum class WindowType
	{
		Default
	};

	// 窗口注册器类，负责注册窗口
	class WindowClassRegister
	{
	public:
		static const WindowClassRegister* GetInstance();

	private:
		WindowClassRegister();
		WindowClassRegister(WindowClassRegister&) = delete;
		void operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		std::unordered_map<WindowType, std::string> windowClassMap;

	private:
		// 窗口注册器单例
		static WindowClassRegister* instance;
		// 互斥锁，用于多线程访问窗口注册器单例
		static std::mutex mutex;
	};

	class Window
	{
	public:
		Window(HINSTANCE hInstance, float width, float height, WindowType type = WindowType::Default);
		~Window();

	public:
		HWND hwnd;
		HINSTANCE hInstance;
	};
}