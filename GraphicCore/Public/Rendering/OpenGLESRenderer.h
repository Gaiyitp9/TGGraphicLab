/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Renderer.h"
#include "DynamicGles.h"
#include "Base/Window.h"

namespace TG
{
    class OpenGLESRenderer final : public Renderer
    {
    public:
        OpenGLESRenderer(NativeWindowHandle handle, NativeDeviceContext context);
        ~OpenGLESRenderer() override;

        void Render() override;
        void Present() override;

    private:
        void InitializeTriangle();

        EGLDisplay m_eglDisplay{ EGL_NO_DISPLAY };
        EGLContext m_eglContext{ EGL_NO_CONTEXT };
        EGLSurface m_eglSurface{ EGL_NO_SURFACE };
        bool m_getDisplay{ false };
        bool m_createContext{ false };
        bool m_createSurface{ false };

        GLuint m_shaderProgram{};
        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
    };
}
