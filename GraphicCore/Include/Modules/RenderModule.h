/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "DynamicGles.h"

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

    private:
        bool CreateEGLDisplay();
        bool ChooseEGLConfig();
        bool CreateEGLSurface();
        bool SetupEGLContext();
        void InitialTriangle();

        EGLDisplay m_eglDisplay{};
        EGLConfig m_eglConfig{};
        EGLSurface m_eglSurface{};
        EGLContext m_eglContext{};

        GLuint m_shaderProgram{};
        GLuint m_VAO{};
        GLuint m_VBO{};
    };
}
