/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG::PAL::Utility
{
    std::wstring Utf8ToUtf16(std::string_view str);
    std::string Utf16ToUtf8(std::wstring_view wstr);
    std::string ToLower(std::string_view str);
    std::wstring ToLower(std::wstring_view wstr);
    std::string GetBasePath(std::string_view filePath);
    std::wstring GetBasePath(std::wstring_view filePath);
    std::string RemoveBasePath(std::string_view filePath);
    std::wstring RemoveBasePath(std::wstring_view filePath);
    std::string GetFileExtension(std::string_view filePath);
    std::wstring GetFileExtension(std::wstring_view filePath);
    std::string RemoveExtension(std::string_view filePath);
    std::wstring RemoveExtension(std::wstring_view filePath);
}
