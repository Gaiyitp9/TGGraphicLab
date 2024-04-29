/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/Editor/WindowHelper.h"
#include "PAL/Windows/Editor/Window.h"
#include "Diagnostics/Win32Exception.h"
#include <unordered_map>
#include <format>

#define REGISTER_MESSAGE(msg) {msg, #msg}

namespace TG
{
    static LRESULT WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // 注：窗口处理函数不能向上传递异常
        auto* const pWnd = reinterpret_cast<Window* const>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        return pWnd->WindowProc(hwnd, msg, wParam, lParam);
    }

    static LRESULT WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // 注：窗口处理函数不能向上传递异常
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* const pWnd = static_cast<Window* const>(pCreate->lpCreateParams);

            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcThunk));

            return pWnd->WindowProc(hwnd, msg, wParam, lParam);
        }
        // 处理WM_NCCREATE之前的消息
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    // 记录不同类型窗口的名称
    static std::unordered_map<WindowType, wchar_t const*> gs_windowClassName;

    // 注册窗口类
    static int RegisterWindow()
    {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WindowProcSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = nullptr;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.hIconSm = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = L"Default";

        if (RegisterClassExW(&wc) == 0)
            CheckLastError();

        gs_windowClassName[WindowType::Default] = L"Default";

        return 0;
    }

    // 调用RegisterWindow函数
    [[maybe_unused]] static int gs_registerWnd = RegisterWindow();

    wchar_t const* GetWindowClassName(const WindowType& type)
    {
        return gs_windowClassName.at(type);
    }

    std::string GetWindowMessageInfo(UINT msg, WPARAM wp, LPARAM lp)
    {
        // 窗口消息m_windowMessage
        static std::unordered_map<DWORD, char const*> s_windowMessage
        {
                REGISTER_MESSAGE(WM_NULL),
                REGISTER_MESSAGE(WM_CREATE),
                REGISTER_MESSAGE(WM_DESTROY),
                REGISTER_MESSAGE(WM_MOVE),
                REGISTER_MESSAGE(WM_SIZE),
                REGISTER_MESSAGE(WM_ACTIVATE),
                REGISTER_MESSAGE(WM_SETFOCUS),
                REGISTER_MESSAGE(WM_KILLFOCUS),
                REGISTER_MESSAGE(WM_ENABLE),
                REGISTER_MESSAGE(WM_SETREDRAW),
                REGISTER_MESSAGE(WM_SETTEXT),
                REGISTER_MESSAGE(WM_GETTEXT),
                REGISTER_MESSAGE(WM_GETTEXTLENGTH),
                REGISTER_MESSAGE(WM_PAINT),
                REGISTER_MESSAGE(WM_CLOSE),
                REGISTER_MESSAGE(WM_QUERYENDSESSION),
                REGISTER_MESSAGE(WM_QUIT),
                REGISTER_MESSAGE(WM_QUERYOPEN),
                REGISTER_MESSAGE(WM_ERASEBKGND),
                REGISTER_MESSAGE(WM_SYSCOLORCHANGE),
                REGISTER_MESSAGE(WM_ENDSESSION),
                REGISTER_MESSAGE(WM_SHOWWINDOW),
                REGISTER_MESSAGE(WM_CTLCOLORMSGBOX),
                REGISTER_MESSAGE(WM_CTLCOLOREDIT),
                REGISTER_MESSAGE(WM_CTLCOLORLISTBOX),
                REGISTER_MESSAGE(WM_CTLCOLORBTN),
                REGISTER_MESSAGE(WM_CTLCOLORDLG),
                REGISTER_MESSAGE(WM_CTLCOLORSCROLLBAR),
                REGISTER_MESSAGE(WM_CTLCOLORSTATIC),
                REGISTER_MESSAGE(WM_WININICHANGE),
                REGISTER_MESSAGE(WM_SETTINGCHANGE),
                REGISTER_MESSAGE(WM_DEVMODECHANGE),
                REGISTER_MESSAGE(WM_ACTIVATEAPP),
                REGISTER_MESSAGE(WM_FONTCHANGE),
                REGISTER_MESSAGE(WM_TIMECHANGE),
                REGISTER_MESSAGE(WM_CANCELMODE),
                REGISTER_MESSAGE(WM_SETCURSOR),
                REGISTER_MESSAGE(WM_MOUSEACTIVATE),
                REGISTER_MESSAGE(WM_CHILDACTIVATE),
                REGISTER_MESSAGE(WM_QUEUESYNC),
                REGISTER_MESSAGE(WM_GETMINMAXINFO),
                REGISTER_MESSAGE(WM_ICONERASEBKGND),
                REGISTER_MESSAGE(WM_NEXTDLGCTL),
                REGISTER_MESSAGE(WM_SPOOLERSTATUS),
                REGISTER_MESSAGE(WM_DRAWITEM),
                REGISTER_MESSAGE(WM_MEASUREITEM),
                REGISTER_MESSAGE(WM_DELETEITEM),
                REGISTER_MESSAGE(WM_VKEYTOITEM),
                REGISTER_MESSAGE(WM_CHARTOITEM),
                REGISTER_MESSAGE(WM_SETFONT),
                REGISTER_MESSAGE(WM_GETFONT),
                REGISTER_MESSAGE(WM_QUERYDRAGICON),
                REGISTER_MESSAGE(WM_COMPAREITEM),
                REGISTER_MESSAGE(WM_GETOBJECT),
                REGISTER_MESSAGE(WM_COMPACTING),
                REGISTER_MESSAGE(WM_NCCREATE),
                REGISTER_MESSAGE(WM_NCDESTROY),
                REGISTER_MESSAGE(WM_NCCALCSIZE),
                REGISTER_MESSAGE(WM_NCHITTEST),
                REGISTER_MESSAGE(WM_NCPAINT),
                REGISTER_MESSAGE(WM_NCACTIVATE),
                REGISTER_MESSAGE(WM_GETDLGCODE),
                REGISTER_MESSAGE(WM_NCMOUSEMOVE),
                REGISTER_MESSAGE(WM_NCLBUTTONDOWN),
                REGISTER_MESSAGE(WM_NCLBUTTONUP),
                REGISTER_MESSAGE(WM_NCLBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_NCRBUTTONDOWN),
                REGISTER_MESSAGE(WM_NCRBUTTONUP),
                REGISTER_MESSAGE(WM_NCRBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_NCMBUTTONDOWN),
                REGISTER_MESSAGE(WM_NCMBUTTONUP),
                REGISTER_MESSAGE(WM_NCMBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_KEYDOWN),
                REGISTER_MESSAGE(WM_KEYUP),
                REGISTER_MESSAGE(WM_CHAR),
                REGISTER_MESSAGE(WM_DEADCHAR),
                REGISTER_MESSAGE(WM_SYSKEYDOWN),
                REGISTER_MESSAGE(WM_SYSKEYUP),
                REGISTER_MESSAGE(WM_SYSCHAR),
                REGISTER_MESSAGE(WM_SYSDEADCHAR),
                REGISTER_MESSAGE(WM_KEYLAST),
                REGISTER_MESSAGE(WM_INITDIALOG),
                REGISTER_MESSAGE(WM_COMMAND),
                REGISTER_MESSAGE(WM_SYSCOMMAND),
                REGISTER_MESSAGE(WM_TIMER),
                REGISTER_MESSAGE(WM_HSCROLL),
                REGISTER_MESSAGE(WM_VSCROLL),
                REGISTER_MESSAGE(WM_INITMENU),
                REGISTER_MESSAGE(WM_INITMENUPOPUP),
                REGISTER_MESSAGE(WM_MENUSELECT),
                REGISTER_MESSAGE(WM_MENUCHAR),
                REGISTER_MESSAGE(WM_ENTERIDLE),
                REGISTER_MESSAGE(WM_MOUSEWHEEL),
                REGISTER_MESSAGE(WM_MOUSEMOVE),
                REGISTER_MESSAGE(WM_LBUTTONDOWN),
                REGISTER_MESSAGE(WM_LBUTTONUP),
                REGISTER_MESSAGE(WM_LBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_RBUTTONDOWN),
                REGISTER_MESSAGE(WM_RBUTTONUP),
                REGISTER_MESSAGE(WM_RBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_MBUTTONDOWN),
                REGISTER_MESSAGE(WM_MBUTTONUP),
                REGISTER_MESSAGE(WM_MBUTTONDBLCLK),
                REGISTER_MESSAGE(WM_PARENTNOTIFY),
                REGISTER_MESSAGE(WM_MDICREATE),
                REGISTER_MESSAGE(WM_MDIDESTROY),
                REGISTER_MESSAGE(WM_MDIACTIVATE),
                REGISTER_MESSAGE(WM_MDIRESTORE),
                REGISTER_MESSAGE(WM_MDINEXT),
                REGISTER_MESSAGE(WM_MDIMAXIMIZE),
                REGISTER_MESSAGE(WM_MDITILE),
                REGISTER_MESSAGE(WM_MDICASCADE),
                REGISTER_MESSAGE(WM_MDIICONARRANGE),
                REGISTER_MESSAGE(WM_MDIGETACTIVE),
                REGISTER_MESSAGE(WM_MDISETMENU),
                REGISTER_MESSAGE(WM_CUT),
                REGISTER_MESSAGE(WM_COPYDATA),
                REGISTER_MESSAGE(WM_COPY),
                REGISTER_MESSAGE(WM_PASTE),
                REGISTER_MESSAGE(WM_CLEAR),
                REGISTER_MESSAGE(WM_UNDO),
                REGISTER_MESSAGE(WM_RENDERFORMAT),
                REGISTER_MESSAGE(WM_RENDERALLFORMATS),
                REGISTER_MESSAGE(WM_DESTROYCLIPBOARD),
                REGISTER_MESSAGE(WM_DRAWCLIPBOARD),
                REGISTER_MESSAGE(WM_PAINTCLIPBOARD),
                REGISTER_MESSAGE(WM_VSCROLLCLIPBOARD),
                REGISTER_MESSAGE(WM_SIZECLIPBOARD),
                REGISTER_MESSAGE(WM_ASKCBFORMATNAME),
                REGISTER_MESSAGE(WM_CHANGECBCHAIN),
                REGISTER_MESSAGE(WM_HSCROLLCLIPBOARD),
                REGISTER_MESSAGE(WM_QUERYNEWPALETTE),
                REGISTER_MESSAGE(WM_PALETTEISCHANGING),
                REGISTER_MESSAGE(WM_PALETTECHANGED),
                REGISTER_MESSAGE(WM_DROPFILES),
                REGISTER_MESSAGE(WM_POWER),
                REGISTER_MESSAGE(WM_WINDOWPOSCHANGED),
                REGISTER_MESSAGE(WM_WINDOWPOSCHANGING),
                REGISTER_MESSAGE(WM_HELP),
                REGISTER_MESSAGE(WM_NOTIFY),
                REGISTER_MESSAGE(WM_CONTEXTMENU),
                REGISTER_MESSAGE(WM_TCARD),
                REGISTER_MESSAGE(WM_MDIREFRESHMENU),
                REGISTER_MESSAGE(WM_MOVING),
                REGISTER_MESSAGE(WM_STYLECHANGED),
                REGISTER_MESSAGE(WM_STYLECHANGING),
                REGISTER_MESSAGE(WM_SIZING),
                REGISTER_MESSAGE(WM_SETHOTKEY),
                REGISTER_MESSAGE(WM_PRINT),
                REGISTER_MESSAGE(WM_PRINTCLIENT),
                REGISTER_MESSAGE(WM_POWERBROADCAST),
                REGISTER_MESSAGE(WM_HOTKEY),
                REGISTER_MESSAGE(WM_GETICON),
                REGISTER_MESSAGE(WM_EXITMENULOOP),
                REGISTER_MESSAGE(WM_ENTERMENULOOP),
                REGISTER_MESSAGE(WM_DISPLAYCHANGE),
                REGISTER_MESSAGE(WM_STYLECHANGED),
                REGISTER_MESSAGE(WM_STYLECHANGING),
                REGISTER_MESSAGE(WM_GETICON),
                REGISTER_MESSAGE(WM_SETICON),
                REGISTER_MESSAGE(WM_SIZING),
                REGISTER_MESSAGE(WM_MOVING),
                REGISTER_MESSAGE(WM_CAPTURECHANGED),
                REGISTER_MESSAGE(WM_DEVICECHANGE),
                REGISTER_MESSAGE(WM_PRINT),
                REGISTER_MESSAGE(WM_PRINTCLIENT),
                REGISTER_MESSAGE(WM_IME_SETCONTEXT),
                REGISTER_MESSAGE(WM_IME_NOTIFY),
                REGISTER_MESSAGE(WM_NCMOUSELEAVE),
                REGISTER_MESSAGE(WM_EXITSIZEMOVE),
                REGISTER_MESSAGE(WM_DWMNCRENDERINGCHANGED),
                REGISTER_MESSAGE(WM_ENTERSIZEMOVE),
        };

        const auto it = s_windowMessage.find(msg);
        const std::string& msgName = it == s_windowMessage.end() ?
                                      std::format("Unknown message: {:#x}", msg) :
                                      it->second;
        return std::format("{:<25} LP: {:#018x}   WP: {:#018x}", msgName, lp, wp);
    }
}
