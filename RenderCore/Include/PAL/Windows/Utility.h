/****************************************************************
* TianGong GraphicLab											*
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
    std::string GetBasePath(const std::string &filePath);
    std::wstring GetBasePath(const std::wstring &filePath);
    std::string RemoveBasePath(const std::string &filePath);
    std::wstring RemoveBasePath(const std::wstring &filePath);
    std::string GetFileExtension(const std::string &filePath);
    std::wstring GetFileExtension(const std::wstring &filePath);
    std::string RemoveExtension(const std::string &filePath);
    std::wstring RemoveExtension(const std::wstring &filePath);
}
