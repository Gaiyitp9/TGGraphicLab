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
        std::setlocale(LC_ALL, ".utf-8");
        std::size_t len = 0;
        mbstowcs_s(&len, nullptr, 0, str.data(), 0);
        std::wstring wstr(len, L' ');
        mbstowcs_s(&len, wstr.data(), len, str.data(), len);
        return wstr;
    }

    std::string WideCharsToMultiBytes(std::wstring_view wstr)
    {
        std::setlocale(LC_ALL, ".utf-8");
        std::size_t len = 0;
        wcstombs_s(&len, nullptr, 0, wstr.data(), 0);
        std::string str(len, ' ');
        wcstombs_s(&len, str.data(), len, wstr.data(), len);
        return str;
    }

    // 这里仅留作记录
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
