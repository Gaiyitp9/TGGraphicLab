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

#include "pch.h"
#include <chrono>

namespace LCH
{
	class Time
	{
	public:
		Time();
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;
		~Time();

		inline float DeltaTime() const { return deltaTime; }
		inline float TotalTime() const { return totalTime; }
		std::wstring Now() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		float totalTime;
		float deltaTime;

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;
		std::chrono::steady_clock::time_point last;
	};
}