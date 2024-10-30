/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "DynamicGles.h"
#include "Base/Window.h"

namespace TG
{
    class RenderModule final : public Module
    {
    public:
        RenderModule();
        RenderModule(const RenderModule&) = delete;
        RenderModule(RenderModule&&) = delete;
        RenderModule& operator=(const RenderModule&) = delete;
        RenderModule& operator=(RenderModule&&) = delete;
        ~RenderModule() override;

        void Update() override;

        bool PlugInVideoDisplay(const IVideoDisplay& display);

    private:
        void InitialTriangle();

        EGLDisplay m_eglDisplay{ nullptr };
        EGLConfig m_eglConfig{ nullptr };
        EGLContext m_eglContext{ nullptr };
        EGLSurface m_eglSurface{ nullptr };

        GLuint m_shaderProgram{};
        GLuint m_VAO{};
        GLuint m_VBO{};
    };
}
