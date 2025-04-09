/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    struct ITimer
    {
        virtual ~ITimer() = default;

        [[nodiscard]] virtual float GetTime() const = 0;
    };
}
