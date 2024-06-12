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
        explicit BaseException(std::string_view description = "No Description");
        ~BaseException() override = default;

        [[nodiscard]] char const* what() const override;

    private:
        std::stacktrace m_stackTrace{ std::stacktrace::current() };
        std::string m_whatBuffer;
    };
}
