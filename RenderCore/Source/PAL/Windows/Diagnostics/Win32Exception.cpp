/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/Diagnostics/Win32Exception.h"
#include "PAL/Windows/Utility.h"
#include <format>

namespace TG::PAL
{
	Win32Exception::Win32Exception(HRESULT hr, const std::wstring& description)
		: BaseException(description), m_errorCode(hr)
	{
		// 提取错误码中的信息
		TranslateHrErrorCode();
	}

	Win32Exception::~Win32Exception() = default;

	char const* Win32Exception::what() const
	{
		static std::string whatBuffer;

		// 记录异常信息
        std::wstring wWhatBuffer = std::format(L"Exception type: Windows API Exception\n");
        wWhatBuffer += std::format(L"HRESULT: {:#010x}\nError Message: {}", m_errorCode, m_errorMsg);
        wWhatBuffer += m_description;
        wWhatBuffer += Separator;
		for (const auto& [index, file, function, line] : m_stackFrameInfo)
		{
            wWhatBuffer += std::format(L"Frame: {}\nFile: {}\nFunction: {}\nLine: {}",
								index, file, function, line);
            wWhatBuffer += Separator;
		}
        whatBuffer = Utility::Utf16ToUtf8(wWhatBuffer);
		return whatBuffer.c_str();
	}

	void Win32Exception::TranslateHrErrorCode()
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
		DWORD msgLen = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                      nullptr, m_errorCode, LANG_SYSTEM_DEFAULT, msgBuf, 256, nullptr);
        m_errorMsg = msgLen > 0 ? msgBuf : L"Unidentified error code";
	}
}
