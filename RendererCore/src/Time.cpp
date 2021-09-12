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
#include "Time.h"

namespace LCH
{
	Time::Time()
		: totalTime(0.0f), deltaTime(0.0f)
	{

	}

	Time::~Time()
	{

	}

	std::wstring Time::Now() const
	{
		std::chrono::system_clock::time_point t = wallClock.now();
		auto const localT = std::chrono::current_zone()->to_local(t);
		auto const days = std::chrono::floor<std::chrono::days>(localT);
		auto const hhmmss = std::chrono::duration_cast<std::chrono::seconds>(localT - days);
		return std::format(L"{}", std::chrono::hh_mm_ss(hhmmss));
	}

	void Time::Tick()
	{
		std::chrono::steady_clock::time_point t = timer.now();
	}
}