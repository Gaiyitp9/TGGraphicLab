/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformHeaders.h"
#include "../Singleton.hpp"
#include <unordered_map>
#include <string>

namespace TG
{
	// 窗口种类
	enum class WindowType
	{
		Default
	};

	// 窗口注册器类，负责注册窗口和消息
	class WindowRegister : public Singleton<WindowRegister>
	{
	public:
		WindowRegister();
		~WindowRegister();

		HINSTANCE hInstance() const noexcept;
		const std::wstring& GetWindowClassName(const WindowType& type) const;
		std::wstring GetWindowMesssageInfo(const std::wstring& window, UINT msg, WPARAM wp, LPARAM lp) const;

	private:
		static LRESULT CALLBACK WindowProcSetup(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK WindowProcThunk(HWND, UINT, WPARAM, LPARAM);

	private:
		HINSTANCE m_hInstance;											// 程序文件(.exe)句柄
		std::unordered_map<WindowType, std::wstring> m_windowClassName;	// 记录不同类型窗口的名称
		std::unordered_map<DWORD, std::wstring> m_windowMessage;		// 窗口消息
	};
}