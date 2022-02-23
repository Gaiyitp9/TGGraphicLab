/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
#include <queue>
#include "InputEvent.h"

namespace LCH
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();

		void OnMouseMove(int x, int y);
		void OnMouseEnter();

	private:
		std::queue<InputEvent> events;
	};
}