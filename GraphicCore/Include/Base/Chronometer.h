/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <chrono>

namespace TG
{
	// 高精度计时器
	class Chronometer
	{
	public:
		Chronometer();
		Chronometer(const Chronometer&) = delete;
		Chronometer(Chronometer&&) = delete;
		Chronometer& operator=(const Chronometer&) = delete;
		Chronometer& operator=(Chronometer&&) = delete;
		~Chronometer();

		static std::string Date();                             // 当前时区的日期和时间

		[[nodiscard]] float DeltaTime() const;
		[[nodiscard]] float TotalTime() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		double m_deltaTime{ 0.0 };	// 单位毫秒(ms)
		bool m_stopped{ false };

		std::chrono::steady_clock::time_point m_base{ std::chrono::steady_clock::now() };	// 开始运行的时间点
		std::chrono::steady_clock::time_point m_last{ std::chrono::steady_clock::now() };   // 上一帧的时间点
		std::chrono::steady_clock::time_point m_stop{ std::chrono::steady_clock::now() };   // 暂停的时间点
		std::chrono::duration<double, std::milli> m_paused{ 0.0 };	// 已暂停的时间
	};
}
