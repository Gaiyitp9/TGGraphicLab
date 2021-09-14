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
		TimeSystem(const TimeSystem&) = delete;
		TimeSystem& operator=(const TimeSystem&) = delete;
		~TimeSystem();

		float DeltaTime() const { return static_cast<float>(deltaTime); }
		float TotalTime() const { return totalTime; }
		std::wstring Time() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		float totalTime;	
		double deltaTime;		// µ•Œª∫¡√Î(ms)

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;
		std::chrono::steady_clock::time_point base;
		std::chrono::steady_clock::time_point last;
		std::chrono::steady_clock::time_point stop;
		std::chrono::duration<double, std::milli> paused;
	};
}