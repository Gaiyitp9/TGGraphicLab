/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Diagnostics/BaseException.h"

namespace TG
{
    BaseException::BaseException(char const* description)
        : m_description(description)
    {}

    char const* BaseException::what() const
    {
        static std::string whatBuffer;

        whatBuffer = std::format("Exception type: Base Exception\n"
                                 "{}\n"
                                 "{}\n", m_description, m_stackTrace);
        return whatBuffer.c_str();
    }
}
