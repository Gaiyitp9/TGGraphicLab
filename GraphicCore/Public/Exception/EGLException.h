/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Exception/BaseException.h"

namespace TG
{
    class EGLException final : public BaseException
    {
    public:
        ~EGLException() override = default;

        static EGLException Create(std::string_view message = "");

    private:
        explicit EGLException(std::string_view message);
    };
}
