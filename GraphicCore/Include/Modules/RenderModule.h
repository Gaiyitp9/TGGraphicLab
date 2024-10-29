/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "DynamicGles.h"
#include "Base/Window.h"
#include "imgui_impl_opengl3.h"

namespace TG
{
    struct EGLData
    {
        EGLDisplay display;
        EGLContext context;
        EGLSurface surface;
    };

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

        static void HookRendererCreateWindow(ImGuiViewport* viewport);
        static void HookRendererDestroyWindow(ImGuiViewport* viewport);
        static void HookPlatformRenderWindow(ImGuiViewport* viewport, void*);
        static void HookRendererSwapBuffers(ImGuiViewport* viewport, void*);

        using Win32Proc = LRESULT (WINAPI*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static Win32Proc PrevWndProc;

        static EGLDisplay m_eglDisplay;
        static EGLConfig m_eglConfig;
        static EGLContext m_eglContext;
        EGLSurface m_eglSurface{ nullptr };

        GLuint m_shaderProgram{};
        GLuint m_VAO{};
        GLuint m_VBO{};
    };
}
