/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    enum class GraphicsAPI : unsigned char
    {
        OpenGLES,
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
    };

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void Render() = 0;
        virtual void Present() = 0;
        virtual void FrameBufferResizeCallback(unsigned int width, unsigned int height) = 0;
    };
}
