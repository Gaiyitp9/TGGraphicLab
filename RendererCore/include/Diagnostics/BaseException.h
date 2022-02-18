/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include <exception>
#include <string>
#include <vector>

namespace LCH
{

	class BaseException : public std::exception
	{
		struct StackFrame
		{
			unsigned int index;
			std::string file;
			std::string function;
			unsigned int line;
		};

	public:
		BaseException(std::string description);
		~BaseException();

		virtual char const* what() const override;
		virtual char const* GetType() const noexcept;

	protected:
		std::string whatBuffer;							// 异常信息
		std::vector<StackFrame> stackFrameInfo;			// 栈帧信息

	private:
		void StackTrace();								// 追踪栈帧信息

		HANDLE hProcess;								// 当前进程句柄
		constexpr static USHORT FramesToCapture = 100;	// 最大追踪栈帧数
		constexpr static UINT MaxNameLen = 255;			// 函数名称最大长度
	};
}