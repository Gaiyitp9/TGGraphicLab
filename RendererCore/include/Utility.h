/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/
#pragma once

#include "CustomWindows.h"
#include <locale>

namespace LCH
{
	class Utility
	{
	private:
		Utility();

	public:
		static std::wstring AnsiToWideString(const std::string& str);
		static std::string WideStringToAnsi(const std::wstring& wstr);
		static std::string ToLower(const std::string& str);
		static std::wstring ToLower(const std::wstring& wstr);
		static std::string GetBasePath(const std::string& str);
		static std::wstring GetBasePath(const std::wstring& wstr);
		static std::string RemoveBasePath(const std::string& str);
		static std::wstring RemoveBasePath(const std::wstring& wstr);
		static std::string GetFileExtension(const std::string& str);
		static std::wstring GetFileExtension(const std::wstring& wstr);
		static std::string RemoveExtension(const std::string& str);
		static std::wstring RemoveExtension(const std::wstring& wstr);
	};
}