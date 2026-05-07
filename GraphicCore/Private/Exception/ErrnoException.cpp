/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Exception/ErrnoException.h"
#include <cstring>
#include <format>
#include <stacktrace>

namespace TG
{
    ErrnoException::ErrnoException(std::string_view message) : BaseException(message) {}

    ErrnoException ErrnoException::Create(std::string_view message)
    {
        std::string whatBuffer = std::format("Exception type: System call Exception\n"
                                            "Errno: {:#04x}\nError Message: {}\n"
                                            "{}\n"
                                            "{}\n", errno, strerror(errno), message, std::stacktrace::current());
        return ErrnoException(whatBuffer);
    }
}
