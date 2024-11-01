/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Exception/Windows/Win32Exception.h"
#include "Base/Utility.h"
#include <format>

namespace TG
{
	Win32Exception::Win32Exception(HRESULT errorCode, std::string_view description)
	{
		// 提取错误码中的信息
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
									  nullptr, errorCode, LANG_SYSTEM_DEFAULT, msgBuf, 256, nullptr);
		std::string errorMsg = msgLen > 0 ? WideCharsToMultiBytes(msgBuf) : "Unidentified error code";

		m_whatBuffer = std::format("Exception type: Windows API Exception\n"
								 "HRESULT: {:#010x}\nError Message: {}"
								 "{}"
								 "{}\n", errorCode, errorMsg, description, m_stackTrace);
	}

	char const* Win32Exception::what() const
	{
		return m_whatBuffer.c_str();
	}
}
