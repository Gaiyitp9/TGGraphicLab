/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
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
			std::wstring file;
			std::wstring function;
			unsigned int line;
		};

	public:
		BaseException(const std::wstring& description = L"No Description");
		~BaseException();

		virtual char const* what() const override;
		virtual wchar_t const* GetType() const noexcept;

	protected:
		mutable std::string m_whatBuffer;								// 异常信息
		std::wstring m_description;										// 异常描述
		std::vector<StackFrame> m_stackFrameInfo;						// 栈帧信息
		constexpr static wchar_t const* SEPARATOR =						// 分隔符
			L"\n-----------------------------------------------\n";	

	private:
		void StackTrace();												// 追踪栈帧信息

		HANDLE m_hProcess;												// 当前进程句柄
		constexpr static USHORT FRAMESTOCAPTURE = 100;					// 最大追踪栈帧数
		constexpr static UINT MAXNAMELEN = 255;							// 函数名称最大长度
	};
}