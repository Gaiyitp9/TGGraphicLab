/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "BaseException.hpp"

namespace TG
{
	class WinAPIException : public BaseException
	{
	public:
		explicit WinAPIException(HRESULT hr, const std::wstring& description = L"");
		~WinAPIException() override;

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
            throw WinAPIException(hr, description);
    }

    inline void CheckLastError(const std::wstring& description = L"")
    {
        auto hr = static_cast<HRESULT>(GetLastError());
        throw WinAPIException(hr, description);
    }
}