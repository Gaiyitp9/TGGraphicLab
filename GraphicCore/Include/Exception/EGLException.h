/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "BaseException.h"

namespace TG
{
    class EGLException final : public BaseException
    {
    public:
        explicit EGLException(std::string_view description = "");
        ~EGLException() override = default;

        [[nodiscard]] char const* what() const override;
    };
}
