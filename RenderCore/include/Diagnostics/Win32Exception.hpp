/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "BaseException.hpp"

namespace TG
{
	class Win32Exception : public BaseException
	{
	public:
		explicit Win32Exception(HRESULT hr, const std::wstring& description = L"");
		~Win32Exception() override;

		char const* what() const override;
		wchar_t const* GetType() const noexcept override;

	protected:
		DWORD m_errorCode;
		std::wstring m_errorMsg;

	private:
		void TranslateHrErrorCode();		// 根据错误码生成相应错误信息
	};

    inline void CheckHResult(HRESULT hr, const std::wstring& description = L"")
    {
        if (hr < 0)
            throw Win32Exception(hr, description);
    }

    inline void CheckLastError(const std::wstring& description = L"")
    {
        auto hr = static_cast<HRESULT>(GetLastError());
        throw Win32Exception(hr, description);
    }
}