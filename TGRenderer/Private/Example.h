/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    enum class ExampleEnum
    {
        Triangle,
        Cube,
        BasicLight,
    };

    struct Example
    {
        virtual ~Example() = default;

        virtual void Draw() = 0;
        virtual void DrawUI() = 0;

        virtual void OnViewportChanged(unsigned width, unsigned height) = 0;
    };
}
