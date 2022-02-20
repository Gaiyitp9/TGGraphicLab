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
		HRESULT errorCode;
		std::wstring errorInfo;

	private:
		void TranslateHrErrorCode();
	};
}