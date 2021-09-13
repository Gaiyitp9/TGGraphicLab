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

#include <chrono>

namespace LCH
{
	class TimeSystem
	{
	public:
		TimeSystem();
		~TimeSystem();

		inline float DeltaTime() const { return deltaTime; }
		inline float TotalTime() const { return totalTime; }
		std::wstring Time() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		float totalTime;	// µ•Œª∫¡√Î(ms)
		float deltaTime;

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;
		std::chrono::steady_clock::time_point last;
	};
}