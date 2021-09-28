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
#include "pch.h"

namespace LCH
{
	Keyboard::Keyboard()
	{

	}

	Keyboard::~Keyboard()
	{

	}

	void Keyboard::OnKeyDown(unsigned char key)
	{
		keyDown.set(key, true);
		keyHoldDown.set(key, false);
	}

	void Keyboard::OnKeyHoldDown(unsigned char key)
	{
		keyDown.set(key, true);
		keyHoldDown.set(key, true);
	}

	void Keyboard::OnKeyUp(unsigned char key)
	{
		keyDown.set(key, false);
		keyHoldDown.set(key, false);
	}

	bool Keyboard::GetKeyDown(KeyCode key)
	{
		std::size_t index = static_cast<std::size_t>(key);
		return keyDown[index];
	}

	bool Keyboard::GetKeyHoldDown(KeyCode key)
	{
		std::size_t index = static_cast<std::size_t>(key);
		return keyHoldDown[index];
	}
}