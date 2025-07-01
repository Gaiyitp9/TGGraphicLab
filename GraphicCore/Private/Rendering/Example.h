/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <memory>

namespace TG
{
    struct Example
    {
        virtual ~Example() = default;

        virtual void Render() = 0;
    };

    enum class ExampleEnum
    {
        Quad,
        Cube,
        BasicLight,
    };

    class ExampleFactory
    {
    public:
        virtual ~ExampleFactory() = default;

        virtual std::unique_ptr<Example> CreateExample(ExampleEnum example) = 0;
    };
}
