/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Utility.h"

#include <algorithm>

#include "Exception/BaseException.h"
#include <locale>
#include <fstream>

namespace TG
{
    std::vector<std::string_view> SplitString(std::string_view str, std::string_view delimiter, bool skipEmpty)
    {
        std::vector<std::string_view> tokens;

        if (str.empty())
            return tokens;
        if (delimiter.empty())
        {
            tokens.emplace_back(str);
            return tokens;
        }

        std::size_t pos = 0, next = 0;
        while ((next = str.find(delimiter, pos)) != std::string_view::npos)
        {
            std::string_view token = str.substr(pos, next - pos);
            if (!skipEmpty || !token.empty())
                tokens.push_back(token);
            pos = next + delimiter.size();
        }
        std::string_view last = str.substr(pos);
        if (!skipEmpty || !last.empty())
            tokens.push_back(last);

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
        std::wstring wStr(data);
        std::free(data);
        return wStr;
    }

    std::string WideCharsToMultiBytes(std::wstring_view wStr)
    {
        std::setlocale(LC_ALL, "zh_CN.utf-8");
        // Windows上用安全的版本，Linux没有这个版本，所以用旧版本
#ifdef TG_WINDOWS
        std::size_t length = 0;
        wcstombs_s(&length, nullptr, 0, wStr.data(), 0);
        auto* data = static_cast<char*>(std::malloc(length * sizeof(char)));
        wcstombs_s(&length, data, length, wStr.data(), length);
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

    /**
     * Win32 API 窄字符转宽字符。保留在这里仅作为记录
     */
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

    std::vector<char> LoadBinaryFile(std::string_view fileName)
    {
        std::ifstream file(fileName.data(), std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw BaseException::Create("Failed to open binary file");

        const std::streamsize fileSize = file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    std::string ToLower(std::string_view input)
    {
        std::string lower;
        for (char c : input | std::views::transform([](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }))
        {
            lower += c;
        }
        return lower;
    }

    bool IsImageByExtension(std::string_view extension)
    {
        static const std::vector<std::string> exts = {
            ".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tga",
        };

        std::string ext(extension);
        std::ranges::transform(ext, ext.begin(), ::tolower);
        return std::ranges::find(exts, ext) != exts.end();
    }
}
