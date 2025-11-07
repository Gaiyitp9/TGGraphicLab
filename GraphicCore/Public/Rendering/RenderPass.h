/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Rendering
{
    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void Execute() = 0;
    };
}
