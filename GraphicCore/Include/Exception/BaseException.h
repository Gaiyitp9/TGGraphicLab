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
        BaseException() = default;
        explicit BaseException(std::string_view description);
        ~BaseException() override = default;

        [[nodiscard]] char const* what() const override;

    protected:
        std::string m_whatBuffer;
        std::stacktrace m_stackTrace{ std::stacktrace::current() };
    };
}
