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
	class Keyboard
	{
	public:
		Keyboard();
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		~Keyboard();

		void OnKeyDown();
		void OnKeyHoldDown();
		void OnKeyUp();

		bool GetKeyDown(KeyCode key);
		bool GetKeyHoldDown(KeyCode key);
		bool GetKeyUp(KeyCode key);

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyDown;
		std::bitset<nKeys> keyHoldDown;
		std::bitset<nKeys> keyUp;
	};

	enum class KeyCode : unsigned char
	{
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		J = 73,
	};
}