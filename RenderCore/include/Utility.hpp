/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG::Utility
{
    std::wstring Utf8ToUtf16(const std::string &str);
    std::string Utf16ToUtf8(const std::wstring &wstr);
    std::string ToLower(const std::string &str);
    std::wstring ToLower(const std::wstring &wstr);
    std::string GetBasePath(const std::string &str);
    std::wstring GetBasePath(const std::wstring &wstr);
    std::string RemoveBasePath(const std::string &str);
    std::wstring RemoveBasePath(const std::wstring &wstr);
    std::string GetFileExtension(const std::string &str);
    std::wstring GetFileExtension(const std::wstring &wstr);
    std::string RemoveExtension(const std::string &str);
    std::wstring RemoveExtension(const std::wstring &wstr);
}