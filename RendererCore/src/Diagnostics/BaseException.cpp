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
	BaseException::BaseException(int lineNum, std::wstring filePath)
		: lineNum(lineNum), filePath(filePath)
	{
		// 初始化符号处理器，用于堆栈信息追踪
		hProcess = GetCurrentProcess();
		SymInitializeW(hProcess, nullptr, true);
		hThread = GetCurrentThread();

		std::wstring wstr = GetType() + std::format(L"\n[File] {}\n[Line] {}\n", filePath, lineNum);
		whatBuffer += Utility::WideStringToAnsi(wstr);
	}
	
	BaseException::~BaseException()
	{
		// 释放符号处理器资源
		SymCleanup(hProcess);
	}

	char const* BaseException::what() const
	{
		return whatBuffer.c_str();
	}

	wchar_t const* BaseException::GetType() const noexcept
	{
		return L"Base Exception";
	}

	int BaseException::GetLineNum() const noexcept
	{
		return lineNum;
	}

	const std::wstring& BaseException::GetFilePath() const noexcept
	{
		return filePath;
	}

	void BaseException::StackTrace()
	{
		void* stackFrames[FramesToCapture];
		USHORT frameCount = CaptureStackBackTrace(0, FramesToCapture, stackFrames, nullptr);

		SYMBOL_INFOW* symbol = reinterpret_cast<SYMBOL_INFOW*>(malloc(sizeof(SYMBOL_INFOW) + MaxNameLen * sizeof(WCHAR)));
		symbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
		symbol->MaxNameLen = MaxNameLen;

		IMAGEHLP_LINEW64 imageLine = { 0 };
		imageLine.SizeOfStruct = sizeof(IMAGEHLP_LINEW64);

		for (USHORT i = 0; i < frameCount; ++i)
		{
			SymFromAddrW(hProcess, (DWORD64)stackFrames[i], 0, symbol);
			SymGetLineFromAddrW(hProcess, (DWORD64)stackFrames[i], 0, &imageLine);
		}
	}
}