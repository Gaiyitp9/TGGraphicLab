/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.h"
#include "Base/EventHandler.hpp"
#include <bitset>

namespace TG::Input
{
	class Keyboard : public IEventHandler<Event<Keyboard>>
	{
	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;
		Keyboard& operator=(Keyboard&&) = delete;
		~Keyboard() override = default;

		void Update();
        void Handle(const Event<Keyboard>& event) override;
		[[nodiscard]] bool GetKey(KeyCode key) const;
		[[nodiscard]] bool GetKeyDown(KeyCode key) const;
		[[nodiscard]] bool GetKeyUp(KeyCode key) const;

	private:
		std::bitset<256> m_keyHold;					    // 按键状态(是否被按下)
		std::bitset<256> m_keyDown;					    // 按键是否刚刚按下
		std::bitset<256> m_keyUp;						// 按键是否刚刚松开
	};
}
