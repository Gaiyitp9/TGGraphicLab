/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.hpp"

namespace TG
{
	// 弹出窗口
	class PopupWindow : public Window
	{
	public:
		PopupWindow(int x, int y, int width, int height, std::shared_ptr<Window> parent = {});
		PopupWindow(const PopupWindow&) = delete;
		PopupWindow& operator=(const PopupWindow&) = delete;
		~PopupWindow();

		void Update() override;

	private:
		void Initialize();
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) override;
	};
}