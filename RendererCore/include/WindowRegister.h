/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include <unordered_map>

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
		static WindowRegister& GetInstance();
		void Initialize(const std::vector<int>& icons);					// 初始化注册器，注册所有窗口类
		HINSTANCE GetHInstance() const noexcept;
		const std::wstring& GetWindowClassName(const WindowType& type) const;
		std::wstring GetWindowMesssageInfo(const std::wstring& window, UINT msg, WPARAM wp, LPARAM lp) const;

	private:
		WindowRegister();
		~WindowRegister();
		WindowRegister(const WindowRegister&) = delete;
		WindowRegister& operator=(const WindowRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK WindowProcThunk(HWND, UINT, WPARAM, LPARAM);

	private:
		HINSTANCE hInstance;											// 程序文件(.exe)句柄
		std::unordered_map<WindowType, std::wstring> windowClassName;	// 记录不同类型窗口的名称
		std::unordered_map<DWORD, std::wstring> windowMessage;			// 窗口消息
	};
}