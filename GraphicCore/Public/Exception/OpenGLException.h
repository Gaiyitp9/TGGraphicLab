/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Exception/BaseException.h"

namespace TG
{
    class OpenGLException : public BaseException
    {
    public:
        ~OpenGLException() override = default;

        static OpenGLException Create(std::string_view message = "");

    private:
        explicit OpenGLException(std::string_view description);
    };
}
