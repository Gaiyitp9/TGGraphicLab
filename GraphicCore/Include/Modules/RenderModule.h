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

        void PlugInVideoDisplay(const IVideoDisplay& display);

    private:
        void InitialTriangle();

        EGLDisplay m_eglDisplay{ EGL_NO_DISPLAY };
        EGLContext m_eglContext{ EGL_NO_CONTEXT };
        EGLSurface m_eglSurface{ EGL_NO_SURFACE };

        GLuint m_shaderProgram{};
        GLuint m_VAO{};
        GLuint m_VBO{};
    };
}
