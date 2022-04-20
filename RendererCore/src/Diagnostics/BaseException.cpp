/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/BaseException.h"
#include "Utility.h"
#include <format>
#include <DbgHelp.h>

namespace LCH
{
	BaseException::BaseException(const std::wstring& description)
		: description(description)
	{
		// 初始化符号处理器，用于堆栈信息追踪
		hProcess = GetCurrentProcess();
		SymInitialize(hProcess, nullptr, true);

		// 记录栈帧信息
		StackTrace();
	}
	
	BaseException::~BaseException()
	{
		// 释放符号处理器资源
		SymCleanup(hProcess);
	}

	char const* BaseException::what() const
	{
		// 记录异常信息
		std::wstring wWhatBuffer = std::format(L"Exception type: {}\n", GetType());
		wWhatBuffer += description;
		wWhatBuffer += SEPARATOR;
		for (const auto& info : stackFrameInfo)
		{
			wWhatBuffer += std::format(L"Frame: {}\nFile: {}\nFunction: {}\nLine: {}",
				info.index, info.file, info.function, info.line);
			wWhatBuffer += SEPARATOR;
		}
		whatBuffer = Utility::WideStringToAnsi(wWhatBuffer);
		return whatBuffer.c_str();
	}

	wchar_t const* BaseException::GetType() const noexcept
	{
		return L"Base Exception";
	}

	void BaseException::StackTrace()
	{
		void* stackFrames[FRAMESTOCAPTURE];
		USHORT frameCount = CaptureStackBackTrace(0, FRAMESTOCAPTURE, stackFrames, nullptr);

		SYMBOL_INFOW* symbol = reinterpret_cast<SYMBOL_INFOW*>(malloc(sizeof(SYMBOL_INFOW) + MAXNAMELEN * sizeof(WCHAR)));
		symbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
		symbol->MaxNameLen = MAXNAMELEN;

		IMAGEHLP_LINEW64 imageLine = { 0 };
		imageLine.SizeOfStruct = sizeof(IMAGEHLP_LINEW64);

		DWORD displacement;
		for (USHORT i = 0; i < frameCount; ++i)
		{
			SymFromAddrW(hProcess, (DWORD64)stackFrames[i], nullptr, symbol);
			SymGetLineFromAddrW64(hProcess, (DWORD64)stackFrames[i], &displacement, &imageLine);
			StackFrame frame{ static_cast<unsigned int>(frameCount - i - 1), imageLine.FileName,
				symbol->Name, static_cast<unsigned int>(imageLine.LineNumber) };
			stackFrameInfo.push_back(frame);
		}

		free(symbol);
	}
}