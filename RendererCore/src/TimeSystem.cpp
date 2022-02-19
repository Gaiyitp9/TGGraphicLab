/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "TimeSystem.h"

namespace LCH
{
	TimeSystem::TimeSystem()
	{
		base = timer.now();
		last = timer.now();
		paused = std::chrono::duration<double, std::milli>(0.0);
	}

	TimeSystem::~TimeSystem()
	{

	}

	float TimeSystem::DeltaTime() const
	{
		return static_cast<float>(deltaTime);
	}

	float TimeSystem::TotalTime() const
	{
		std::chrono::duration<double, std::milli> total;
		if (stopped)
		{
			total = stop - base;
		}
		else
		{
			std::chrono::steady_clock::time_point t = timer.now();
			total = t - base;
		}
		total -= paused;
		return static_cast<float>(total.count());
	}

	std::wstring TimeSystem::Time() const
	{
		std::chrono::system_clock::time_point t = wallClock.now();
		auto const localT = std::chrono::current_zone()->to_local(t);
		auto const days = std::chrono::floor<std::chrono::days>(localT);
		auto const hhmmss = std::chrono::duration_cast<std::chrono::seconds>(localT - days);
		return std::format(L"{:%T}", std::chrono::hh_mm_ss(hhmmss));
	}

	void TimeSystem::Reset()
	{
		base = timer.now();
		last = timer.now();
		paused = std::chrono::duration<double, std::milli>(0.0);
		deltaTime = 0;
		stopped = false;
	}

	void TimeSystem::Pause()
	{
		if (!stopped)
		{
			stop = timer.now();
			stopped = true;
		}
	}

	void TimeSystem::Start()
	{
		if (stopped)
		{
			stopped = false;
			std::chrono::steady_clock::time_point t = timer.now();
			paused += t - stop;
			last = t;
		}
	}

	void TimeSystem::Tick()
	{
		if (stopped)
		{
			deltaTime = 0;
			return;
		}

		std::chrono::steady_clock::time_point t = timer.now();
		std::chrono::duration<double, std::milli> delta = t - last;
		deltaTime = delta.count();
		last = t;
	}
}