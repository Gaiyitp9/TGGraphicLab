/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.hpp"
#include <bitset>

namespace TG::Input
{
	class Keyboard : public Device
	{
	public:
		Keyboard();
		~Keyboard() override;

		void Update() override;							// 分析事件队列，设置各种状态
        void Receive(const Event& e) override;
        void SpyEvent(bool enable) override;
		bool GetKey(KeyCode k) override;
		bool GetKeyDown(KeyCode k) override;
		bool GetKeyUp(KeyCode k) override;

    private:
        static bool Contains(KeyCode k);

	private:
		std::bitset<256> m_keyHold;					    // 按键状态(是否被按下)
		std::bitset<256> m_keyDown;					    // 按键是否刚刚按下
		std::bitset<256> m_keyUp;						// 按键是否刚刚松开

		bool m_autoRepeat = true;						// 按住按键是否能重复输入字符
		bool m_spyKeyboard = false;						// 是否监控键盘
	};
}