/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    class Module
    {
    public:
        virtual ~Module() = default;

        virtual void Update() = 0;
    };
}
