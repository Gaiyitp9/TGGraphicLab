/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Win32API.h"
#include <stacktrace>

namespace TG::PAL
{
	class Win32Exception final : public std::exception
	{
	public:
		explicit Win32Exception(HRESULT hr, char const* description = "No Description");
		~Win32Exception() override;

		[[nodiscard]] char const* what() const override;

	protected:
		char const* m_description;
		std::stacktrace m_stackTrace;

		DWORD m_errorCode;
		std::wstring m_errorMsg;

	private:
		void TranslateHrErrorCode();		// 根据错误码生成相应错误信息
	};

    inline void CheckHResult(HRESULT hr, char const* description = "")
    {
        if (hr < 0)
            throw Win32Exception(hr, description);
    }

    inline void CheckLastError(char const* description = "")
    {
        auto hr = static_cast<HRESULT>(GetLastError());
        throw Win32Exception(hr, description);
    }
}
