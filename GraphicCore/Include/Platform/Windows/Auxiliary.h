/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG::Platform
{
    // 注意，运行时字符集必须设置为utf-8才能使用这两个函数
    // 在msvc中需要设置/utf-8选项
    std::wstring Utf8ToUtf16(std::string_view str);
    std::string Utf16ToUtf8(std::wstring_view wstr);
}
