/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Chronometer.h"

namespace TG
{
	Chronometer::Chronometer() = default;

	Chronometer::~Chronometer() = default;

	float Chronometer::DeltaTime() const
	{
		return static_cast<float>(m_deltaTime);
	}

	float Chronometer::TotalTime() const
	{
		std::chrono::duration<double, std::milli> total{};
		if (m_stopped)
		{
			total = m_stop - m_base;
		}
		else
		{
			std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
			total = t - m_base;
		}
		total -= m_paused;
		return static_cast<float>(total.count());
	}

	std::string Chronometer::Date()
	{
		auto const localT = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
        return std::format("{:%Y-%m-%d %X}", localT);
	}

	void Chronometer::Reset()
	{
        m_base = std::chrono::steady_clock::now();
        m_last = std::chrono::steady_clock::now();
        m_paused = std::chrono::duration<double, std::milli>(0.0);
        m_deltaTime = 0;
        m_stopped = false;
	}

	void Chronometer::Pause()
	{
		if (!m_stopped)
		{
            m_stop = std::chrono::steady_clock::now();
            m_stopped = true;
		}
	}

	void Chronometer::Start()
	{
		if (m_stopped)
		{
            m_stopped = false;
			const std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
            m_paused += t - m_stop;
            m_last = t;
		}
	}

	void Chronometer::Tick()
	{
		if (m_stopped)
		{
            m_deltaTime = 0;
			return;
		}

		const std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
		const std::chrono::duration<double, std::milli> delta = t - m_last;
        m_deltaTime = delta.count();
        m_last = t;
	}
}
