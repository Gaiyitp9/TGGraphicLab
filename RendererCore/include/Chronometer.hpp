/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <chrono>

namespace LCH
{
	// 高精度计时器
	class Chronometer
	{
	public:
		Chronometer();
		Chronometer(const Chronometer&) = delete;
		Chronometer& operator=(const Chronometer&) = delete;
		~Chronometer();

		float DeltaTime() const;
		float TotalTime() const;
		std::wstring Time() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		double deltaTime = 0.0;		// 单位毫秒(ms)
		bool stopped = false;

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;		
		std::chrono::steady_clock::time_point base;			// 开始运行的时间点
		std::chrono::steady_clock::time_point last;			// 上一帧的时间点
		std::chrono::steady_clock::time_point stop;			// 暂停的时间点
		std::chrono::duration<double, std::milli> paused;	// 已暂停的时间
	};
}