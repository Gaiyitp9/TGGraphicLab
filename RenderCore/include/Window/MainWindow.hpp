/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.hpp"
#include "Input/Event.hpp"
#include "../Utility.hpp"
#include <functional>

namespace TG
{
	class MainWindow final : public Window
	{
	public:
		MainWindow(int x, int y, int width, int height, wchar_t const* name = L"TG RenderLab", HWND parent = nullptr);
		MainWindow(const MainWindow&) = delete;
        MainWindow(MainWindow&&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
		~MainWindow();

        void SetInput(std::function<void(const Input::Event&)>&& send);                 // 设置输入事件发送接口
        void SetTimer(std::function<void()>&& start, std::function<void()>&& pause);    // 设置计时器开始和暂停接口
		void SetIcon(wchar_t const* iconPath);				                            // 设置窗口的icon
		void SpyMessage(bool enable) noexcept { m_spyMessage = enable; }	            // 捕捉窗口消息

		[[nodiscard]]
        const std::wstring& Name() const noexcept { return m_name; }		            // 窗口名称

	private:
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) override;	            // 消息处理函数

	private:
		std::wstring m_name;				                // 主窗口标题
		bool m_spyMessage = false;			                // 是否监控窗口消息
        std::function<void(const Input::Event&)> m_send;    // 输入消息发送接口
        std::function<void()> m_start;                      // 计时器开始接口
        std::function<void()> m_pause;                      // 计时器暂停接口
	};
}