/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Utility.h"
#include <locale>

namespace TG
{
    std::vector<std::string_view> SplitString(std::string_view str, std::string_view delim)
    {
        std::vector<std::string_view> tokens;
        std::size_t start = 0, end = 0;
        while ((end = str.find(delim, start)) != std::string_view::npos)
        {
            tokens.push_back(str.substr(start, end - start));
            start = end + delim.size();
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }

    std::wstring MultiBytesToWideChars(std::string_view str)
    {
        std::setlocale(LC_ALL, "zh_CN.utf-8");
        // Windows上用安全的版本，Linux没有这个版本，所以用旧版本
#ifdef TG_WINDOWS
        std::size_t length = 0;
        mbstowcs_s(&length, nullptr, 0, str.data(), 0);
        auto* data = static_cast<wchar_t*>(std::malloc(length * sizeof(wchar_t)));
        mbstowcs_s(&length, data, length, str.data(), length);
#else
        // 计算出的长度不包含'/0'，所以要加1
        std::size_t length = std::mbstowcs(nullptr, str.data(), 0) + 1;
        auto* data = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
        std::mbstowcs(data, str.data(), length);
#endif
        std::wstring wstr(data);
        std::free(data);
        return wstr;
    }

    std::string WideCharsToMultiBytes(std::wstring_view wstr)
    {
        std::setlocale(LC_ALL, "zh_CN.utf-8");
        // Windows上用安全的版本，Linux没有这个版本，所以用旧版本
#ifdef TG_WINDOWS
        std::size_t length = 0;
        wcstombs_s(&length, nullptr, 0, wstr.data(), 0);
        auto* data = static_cast<char*>(std::malloc(length * sizeof(char)));
        wcstombs_s(&length, data, length, wstr.data(), length);
#else
        // 计算出的长度不包含'/0'，所以要加1
        std::size_t length = std::wcstombs(nullptr, wstr.data(), 0) + 1;
        auto* data = static_cast<char*>(malloc(length * sizeof(char)));
        std::wcstombs(data, wstr.data(), length);
#endif
        std::string str(data);
        std::free(data);
        return str;
    }

    // Win32 API 窄字符转宽字符。这里仅留作记录。
    // std::wstring Utf8ToUtf16(std::string_view str)
    // {
    //     int length = MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
    //     auto* wide = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
    //     MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, wide, length);
    //     std::wstring wstr(wide);
    //     free(wide);
    //     return wstr;
    // }
    //
    // std::string Utf16ToUtf8(std::wstring_view wstr)
    // {
    //     int length = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
    //     auto* ansi = static_cast<char*>(malloc(length * sizeof(char)));
    //     WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, ansi, length, nullptr, nullptr);
    //     std::string str(ansi);
    //     free(ansi);
    //     return str;
    // }
}
