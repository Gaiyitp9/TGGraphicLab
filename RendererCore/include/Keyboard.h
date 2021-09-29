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
	enum class KeyCode : unsigned char
	{
		A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
		I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
	};

	class Keyboard
	{
	public:
		Keyboard();
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		~Keyboard();

		void OnKeyPressed(unsigned char key);
		void OnKeyReleased(unsigned char key);

		bool AutoRepeatEnable() const noexcept;

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyStates;

		bool autoRepeatEnable = true;
	};
}