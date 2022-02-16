/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "SlimWindows.h"
#include <unordered_map>
#include <mutex>

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
		void Initialize(const std::vector<int>& icons);	// 初始化注册器，注册所有窗口类
		HINSTANCE GetHInstance() const noexcept;
		const std::wstring& GetWindowClassName(const WindowType&) const;
		const std::wstring& GetWindowMesssageInfo(const std::wstring&, DWORD, WPARAM, LPARAM);

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

		std::unordered_map<DWORD, std::wstring> windowMessage;			// 窗口消息
		std::unordered_map<std::wstring, std::wstring> messageInfo;		// 消息包含的信息
		std::wstring msgInfo;											// 消息信息
	};
}