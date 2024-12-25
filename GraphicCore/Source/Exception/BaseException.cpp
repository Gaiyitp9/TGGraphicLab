/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Exception/BaseException.h"
#include <stacktrace>

namespace TG
{
    BaseException::BaseException(std::string_view message) : std::exception(message.data()) {}

    BaseException BaseException::Create(std::string_view message)
    {
        std::string whatBuffer = std::format("Exception type: Base Exception\n"
                                 "{}\n"
                                 "{}\n", message, std::stacktrace::current());
        return BaseException(whatBuffer);
    }
}
