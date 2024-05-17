/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Diagnostics/BaseException.h"

namespace TG
{
    BaseException::BaseException(std::string_view description)
    {
        m_whatBuffer = std::format("Exception type: Base Exception\n"
                                 "{}\n"
                                 "{}\n", description, m_stackTrace);
    }

    char const* BaseException::what() const
    {
        return m_whatBuffer.c_str();
    }
}
