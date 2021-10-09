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
#include "Keyboard.h"

namespace LCH
{
	Keyboard::Keyboard()
	{

	}

	Keyboard::~Keyboard()
	{

	}

	void Keyboard::OnKeyPressed(unsigned char key)
	{
		keyStates.set(key, true);
	}

	void Keyboard::OnKeyReleased(unsigned char key)
	{
		keyStates.set(key, false);
	}

	bool Keyboard::AutoRepeatEnable() const noexcept
	{
		return autoRepeatEnable;
	}
}