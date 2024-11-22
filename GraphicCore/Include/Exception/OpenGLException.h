/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "BaseException.h"

namespace TG
{
    class OpenGLException : public BaseException
    {
    public:
        explicit OpenGLException(std::string_view description = "");
        ~OpenGLException() override = default;

        [[nodiscard]] char const* what() const override;
    };
}
