/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformHeaders.h"
#include <locale>

namespace TG
{
	class Utility
	{
	public:
		Utility() = delete;
        Utility(const Utility&) = delete;

        static WPARAM MapLeftRightKey(WPARAM wParam, LPARAM lParam);        // 映射左右按键(shift, ctrl, alt)
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