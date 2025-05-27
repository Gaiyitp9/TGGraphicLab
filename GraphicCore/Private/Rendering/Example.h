/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    struct Example
    {
        virtual ~Example() = default;

        virtual void Render() = 0;
    };
}
