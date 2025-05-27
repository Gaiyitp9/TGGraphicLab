/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Exception/BaseException.h"

namespace TG
{
    class OpenGLESException : public BaseException
    {
    public:
        ~OpenGLESException() override = default;

        static OpenGLESException Create(std::string_view message = "");

    private:
        explicit OpenGLESException(std::string_view message);
    };
}
