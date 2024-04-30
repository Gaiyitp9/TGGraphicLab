/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <stacktrace>

namespace TG
{
    class BaseException : public std::exception
    {
    public:
        explicit BaseException(char const* description = "No Description");
        ~BaseException() override = default;

        [[nodiscard]] char const* what() const override;

    private:
        char const* m_description;
        std::stacktrace m_stackTrace{ std::stacktrace::current() };
    };
}
