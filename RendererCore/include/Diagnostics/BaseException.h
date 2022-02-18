/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include <exception>
#include <string>

namespace LCH
{
	class BaseException : public std::exception
	{
	public:
		BaseException(int lineNum, std::wstring filePath);
		~BaseException();

		virtual char const* what() const override;
		virtual wchar_t const* GetType() const noexcept;

		int GetLineNum() const noexcept;
		const std::wstring& GetFilePath() const noexcept;

	private:
		void StackTrace();		// 追踪栈帧信息

	protected:
		int lineNum;
		std::wstring filePath;
		std::string whatBuffer;

	private:
		HANDLE hProcess;		// 当前进程句柄
		HANDLE hThread;			// 当前线程句柄
		constexpr static USHORT FramesToCapture = 100;	// 最大追踪栈帧数
		constexpr static UINT MaxNameLen = 255;			// 函数名称最大长度
	};
}