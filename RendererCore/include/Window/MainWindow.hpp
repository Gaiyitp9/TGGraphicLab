/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.hpp"
#include "Input/InputSystem.hpp"
#include "Chronometer.hpp"

namespace LCH
{
	class MainWindow : public Window
	{
	public:
		MainWindow(int x, int y, int width, int height, wchar_t const* name = L"TG RenderLab", std::shared_ptr<Window> parent = {});
		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
		~MainWindow();

		void Update() override;
		void SetIcon(int icon);										// 设置窗口的icon

		const std::wstring& Name() const noexcept;					// 窗口名称
		const InputSystem& Input() const noexcept;					// 获取输入系统

		void SetTimer(const std::shared_ptr<Chronometer>& timer);	// 关联计时器
		void SpyMessage(bool enable) noexcept;						// 捕捉窗口消息
		void SpyInputEvent(bool enable) noexcept;					// 是否监控输入事件

	private:
		void Initialize();
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) override;	// 消息处理函数

	private:
		std::wstring name;					// 主窗口标题
		std::weak_ptr<Chronometer> timer;	// 高精度计时器
		InputSystem input;					// 输入系统，用于记录输入信息，比如键盘和鼠标
		bool spyMessage = false;			// 是否监控窗口消息
	};
}