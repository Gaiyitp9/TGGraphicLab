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

#include "Window.h"

namespace IGGSZLab
{
	WindowClassRegister* WindowClassRegister::instance = nullptr;
	std::mutex WindowClassRegister::mutex;

	WindowClassRegister::WindowClassRegister()
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProcSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(nullptr);
	}

	LRESULT WindowClassRegister::WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcThunk));
			return pWnd->WindowProc(hwnd, msg, wParam, lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT WindowClassRegister::WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWnd->WindowProc(hwnd, msg, wParam, lParam);
	}

	const WindowClassRegister* WindowClassRegister::GetInstance()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr)
			instance = new WindowClassRegister();
		return instance;
	}

	Window::Window(HINSTANCE hInstance, float width, float height, WindowType type)
	{

	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
