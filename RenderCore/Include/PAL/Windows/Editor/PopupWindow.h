/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.h"

namespace TG
{
	// 弹出窗口
	class PopupWindow final : public Window
	{
	public:
		PopupWindow(int x, int y, int width, int height, HWND parent = nullptr);
		PopupWindow(const PopupWindow&) = delete;
		PopupWindow(PopupWindow&&) = delete;
		PopupWindow& operator=(const PopupWindow&) = delete;
		PopupWindow& operator=(PopupWindow&&) = delete;
		~PopupWindow() override;

	private:
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) override;
	};
}
