/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Exception/BaseException.h"

namespace TG
{
    class ErrnoException : public BaseException
    {
    public:
        ~ErrnoException() override = default;

        static ErrnoException Create(std::string_view message = "");

    private:
        explicit ErrnoException(std::string_view message);
    };
}
