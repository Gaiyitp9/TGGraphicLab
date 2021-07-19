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
#include "resource.h"

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
		static WindowClassRegister* const GetInstance();

	private:
		WindowClassRegister();
		~WindowClassRegister();
		WindowClassRegister(const WindowClassRegister&) = delete;
		void operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		// 记录不同类型窗口的名称
		std::unordered_map<WindowType, std::wstring> windowClassMap;
		// 实例句柄
		HINSTANCE hInstance;

	private:
		// 窗口注册器单例
		static WindowClassRegister* instance;
		// 互斥锁，用于多线程访问窗口注册器单例
		static std::mutex mutex;
	};

	// 窗口类
	class Window
	{
	public:
		Window(int width, int height, const wchar_t* title = L"IGGSZ Lab");
		Window(const Window&) = delete;
		void operator=(const Window&) = delete;
		~Window();

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::optional<int> ProcessMessage();

	protected:
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
	};
}