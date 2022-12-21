/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/MemoryDbg.hpp"			// 必须放在文件头部，保证malloc使用的是debug版本
#include "Diagnostics/BaseException.hpp"
#include "Utility.hpp"
#include <format>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

namespace TG
{
	BaseException::BaseException(const std::wstring& description)
		: m_description(description)
	{
		// 初始化符号处理器，用于堆栈信息追踪
		m_hProcess = GetCurrentProcess();
		SymInitialize(m_hProcess, nullptr, true);

		// 记录栈帧信息
		StackTrace();
	}
	
	BaseException::~BaseException()
	{
		// 释放符号处理器资源
		SymCleanup(m_hProcess);
	}

	char const* BaseException::what() const
	{
		// 记录异常信息
		std::wstring wWhatBuffer = std::format(L"Exception type: {}\n", GetType());
		wWhatBuffer += m_description;
		wWhatBuffer += SEPARATOR;
		for (const auto& info : m_stackFrameInfo)
		{
			wWhatBuffer += std::format(L"Frame: {}\nFile: {}\nFunction: {}\nLine: {}",
				info.index, info.file, info.function, info.line);
			wWhatBuffer += SEPARATOR;
		}
		m_whatBuffer = Utility::WideStringToAnsi(wWhatBuffer);
		return m_whatBuffer.c_str();
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
			SymFromAddrW(m_hProcess, (DWORD64)stackFrames[i], nullptr, symbol);
			SymGetLineFromAddrW64(m_hProcess, (DWORD64)stackFrames[i], &displacement, &imageLine);
			StackFrame frame{ static_cast<unsigned int>(frameCount - i - 1), imageLine.FileName,
				symbol->Name, static_cast<unsigned int>(imageLine.LineNumber) };
			m_stackFrameInfo.push_back(frame);
		}

		free(symbol);
	}
}