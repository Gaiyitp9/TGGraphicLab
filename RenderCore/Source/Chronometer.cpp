/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Chronometer.h"

namespace TG
{
	Chronometer::Chronometer()
	{
        m_base = std::chrono::steady_clock::now();
        m_last = std::chrono::steady_clock::now();
        m_paused = std::chrono::duration<double, std::milli>(0.0);
	}

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

	std::wstring Chronometer::Date()
	{
        // c++20的方法，但是调用std::chrono::current_zone()之后会产生内存泄漏
		/*auto const localT = std::chrono::current_zone()
                ->to_local(std::chrono::system_clock::now());
        return std::format(L"{:%Y-%m-%d %X}", localT);*/

        auto now = std::chrono::system_clock::now();
        auto localT = std::chrono::system_clock::to_time_t(now);

        std::wstringstream ss;
        ss << std::put_time(std::localtime(&localT), L"%Y-%m-%d %X");
        return ss.str();
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
