/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/
#pragma once

#include <bitset>

namespace LCH
{
	class InputSystem
	{
	public:
		InputSystem();
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		~InputSystem();

		void OnKeyPressed(unsigned char key);
		void OnKeyReleased(unsigned char key);

		bool AutoRepeatEnable() const noexcept;

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyStates;

		bool autoRepeatEnable = true;
	};
}