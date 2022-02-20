/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/WinAPIException.h"

namespace LCH
{
	WinAPIException::WinAPIException(HRESULT hr, const std::wstring& description)
		: BaseException(L"#"), errorCode(hr)
	{

	}

	void WinAPIException::TranslateHrErrorCode()
	{

	}
}