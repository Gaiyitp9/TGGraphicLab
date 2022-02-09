/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "InputSystem.h"

namespace LCH
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
		WindowClassRegister& operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		std::unordered_map<WindowType, std::wstring> windowClassMap;	// 记录不同类型窗口的名称
		HINSTANCE hInstance;											// 实例句柄

	private:
		static WindowClassRegister* instance;	// 窗口注册器单例
		static std::mutex mutex;				// 互斥锁，用于多线程访问窗口注册器单例
	};

	// 窗口类
	class Window
	{
	public:
		Window(int width, int height, int icon = 0, const wchar_t* title = L"IGGSZ TA Group");
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::optional<int> ProcessMessage();

	public:
		int GetIcon() const;
		void ConnectInputSystem(InputSystem* input);

	protected:
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
		int icon;			// 窗口icon索引，在resource.h文件中定义

		InputSystem* input;	// 输入系统
	};
}