/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "BaseException.h"

namespace LCH
{
	class WinAPIException : public BaseException
	{
	public:
		WinAPIException(HRESULT hr, const std::wstring& description = L"No Description");
		~WinAPIException();

		virtual char const* what() const override;
		virtual wchar_t const* GetType() const noexcept override;

	protected:
		DWORD errorCode;
		std::wstring errorMsg;

	private:
		void TranslateHrErrorCode();		// 根据错误码生成相应错误信息
	};
}