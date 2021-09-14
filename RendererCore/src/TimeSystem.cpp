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
	TimeSystem::TimeSystem()
		: totalTime(0.0f), deltaTime(0.0f)
	{
		base = timer.now();
		last = timer.now();
	}

	TimeSystem::~TimeSystem()
	{

	}

	std::wstring TimeSystem::Time() const
	{
		std::chrono::system_clock::time_point t = wallClock.now();
		auto const localT = std::chrono::current_zone()->to_local(t);
		auto const days = std::chrono::floor<std::chrono::days>(localT);
		auto const hhmmss = std::chrono::duration_cast<std::chrono::seconds>(localT - days);
		return std::format(L"{:%T}", std::chrono::hh_mm_ss(hhmmss));
	}

	void TimeSystem::Tick()
	{
		std::chrono::steady_clock::time_point t = timer.now();
		std::chrono::duration<double, std::milli> delta = t - last;
		deltaTime = delta.count();
		last = t;
	}
}