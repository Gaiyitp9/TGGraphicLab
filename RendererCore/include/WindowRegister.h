/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "pch.h"

namespace LCH
{
	// 窗口种类
	enum class WindowType
	{
		Default
	};

	// 窗口注册器类，负责注册窗口和消息
	class WindowRegister
	{
	public:
		static WindowRegister* const GetInstance();
		HINSTANCE GetHInstance() const noexcept;
		std::wstring GetWindowClassName(const WindowType&) const;
		std::wstring GetWindowMesssageInfo(DWORD, LPARAM, WPARAM) const;

	private:
		WindowRegister();
		~WindowRegister();
		WindowRegister(const WindowRegister&) = delete;
		WindowRegister& operator=(const WindowRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static WindowRegister* instance;								// 窗口注册器单例
		static std::mutex mutex;										// 互斥锁，用于多线程访问窗口注册器单例

		HINSTANCE hInstance;											// 程序文件(.exe)句柄
		std::unordered_map<WindowType, std::wstring> windowClassName;	// 记录不同类型窗口的名称
		std::unordered_map<DWORD, std::wstring> windowMessage;			// 窗口消息名称
	};
}