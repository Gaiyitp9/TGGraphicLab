/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <stdexcept>
#include <string_view>

namespace TG
{
    class BaseException : public std::runtime_error
    {
    public:
        ~BaseException() override = default;

        static BaseException Create(std::string_view message = "");

    protected:
        explicit BaseException(std::string_view message);
    };
}
