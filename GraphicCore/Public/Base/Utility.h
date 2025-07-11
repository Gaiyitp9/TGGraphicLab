/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>
#include <string>
#include <ranges>

namespace TG
{
    // 分隔字符串
    std::vector<std::string_view> SplitString(std::string_view str, std::string_view delim);

    // Windows上char使用gbk或者utf-8(在msvc中传入/utf-8选项)，大小为1字节；wchar_t使用utf-16编码，大小为2字节
    // Linux上char使用utf-8，大小为1字节；wchar_t使用utf-32编码，大小为4字节
    // 注意，运行时字符集必须设置为utf-8才能使用这两个函数
    // 窄字符转宽字符
    std::wstring MultiBytesToWideChars(std::string_view str);
    // 宽字符转窄字符
    std::string WideCharsToMultiBytes(std::wstring_view wStr);

    // 读取二进制文件
    std::vector<char> LoadBinaryFile(std::string_view fileName);

    // 字符串转换为小写
    std::string ToLower(std::string_view input);

    // 通过后缀名判断是否为支持的图片格式
    bool IsImageByExtension(std::string_view extension);
}
