/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/WinAPIException.h"
#include <format>

namespace TG::PAL
{
	WinApiException::WinApiException(HRESULT hr, char const* description)
		: m_description(description), m_stackTrace(std::stacktrace::current()), m_errorCode(hr)
	{
		// 提取错误码中的信息
		TranslateHrErrorCode();
	}

	WinApiException::~WinApiException() = default;

	char const* WinApiException::what() const
	{
		static std::string whatBuffer;

        whatBuffer = std::format("Exception type: Windows API Exception\n"
								 "HRESULT: {:#010x}\nError Message: {}\n"
								 "{}\n"
								 "{}\n", m_errorCode, m_errorMsg, m_description, m_stackTrace);
		return whatBuffer.c_str();
	}

	void WinApiException::TranslateHrErrorCode()
	{
		/*
		注意：FormatMessage中的lpBuffer参数与dwFlags有关。
		如果不包含FORMAT_MESSAGE_ALLOCATE_BUFFER，则写法如下：
		wchar_t msgBuf[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_errorCode, LANG_SYSTEM_DEFAULT, msgBuf, 256, nullptr);

		如果包含FORMAT_MESSAGE_ALLOCATE_BUFFER，则写法如下：
		wchar_t* msgBuf = nullptr;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_errorCode, LANG_SYSTEM_DEFAULT, (wchar_t*)&msgBuf, 0, nullptr);
		LocalFree(msgBuf);

		不同之处在于，lpBuffer需要传入指针的地址，这样才能将分配的内存地址赋给lpBuffer，
		所以要做一个看上去很奇怪的转换：取wchar_t指针的地址，再强转为whar_t指针*/
		wchar_t msgBuf[256];
		const DWORD msgLen = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                      nullptr, m_errorCode, LANG_SYSTEM_DEFAULT, msgBuf, 256, nullptr);
        m_errorMsg = msgLen > 0 ? msgBuf : L"Unidentified error code";
	}
}
