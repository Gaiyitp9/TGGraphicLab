/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef TG_WINDOWS
    #include "Base/Windows/NativeWindow.h"
#endif

namespace TG
{
    // 显示接口
    struct IVideoPort
    {
        virtual ~IVideoPort() = default;

        [[nodiscard]] virtual NativeWindowHandle GetHandle() const noexcept = 0;
        [[nodiscard]] virtual NativeDeviceContext GetContext() const noexcept = 0;
    };

    // 窗口基类
    class Window : public IVideoPort
    {
    public:
        Window(int x, int y, int width, int height, std::string_view name, WindowType type)
            : m_posX(x), m_posY(y), m_width(width), m_height(height),
                m_nativeWindow(x, y, width, height, name, type)
        {}
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window() override = default;

        // 轮询输入事件，需要每帧调用
        static std::optional<int> PollEvents();

        [[nodiscard]] NativeWindowHandle GetHandle() const noexcept override;
        [[nodiscard]] NativeDeviceContext GetContext() const noexcept override;
        [[nodiscard]] bool IsDestroyed() const noexcept;
        [[nodiscard]] int PositionX() const noexcept { return m_posX; }
        [[nodiscard]] int PositionY() const noexcept { return m_posY; }
        [[nodiscard]] int Width() const noexcept { return m_width; }
        [[nodiscard]] int Height() const noexcept { return m_height; }

        void SetIcon(std::string_view iconPath) const;
        void SetPosition(int x, int y);
        void SetSize(int width, int height);
        void Show(bool show) const;

        // 窗口消息事件回调
        void SetWindowPosCallback(const WindowPosDelegate& function);
        void SetWindowSizeCallback(const WindowSizeDelegate& function);
        void SetSuspendCallback(const SuspendDelegate& function);
        void SetResumeCallback(const ResumeDelegate& function);
        void SetKeyCallback(const KeyDelegate& function);
        void SetCharCallback(const CharDelegate& function);
        void SetMouseButtonCallback(const MouseButtonDelegate& function);
        void SetCursorPosCallback(const CursorPosDelegate& function);
        void SetScrollCallback(const ScrollDelegate& function);

    private:
        int m_posX, m_posY;
        int m_width, m_height;
        NativeWindow m_nativeWindow;
    };
}
