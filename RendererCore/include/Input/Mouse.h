/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include "InputEvent.h"
#include <queue>

namespace LCH
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();

		void Update();

		void OnMouseMove(int x, int y);
		void OnMouseEnter();

	private:
		POINT position;
		std::queue<InputEvent> events;
	};
}