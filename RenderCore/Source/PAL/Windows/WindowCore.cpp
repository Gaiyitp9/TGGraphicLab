/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/WindowCore.h"
#include "PAL/Windows/WinAPIException.h"
#include <unordered_map>

namespace TG::PAL
{
    HWND CreateWin32Window(int x, int y, int width, int height, wchar_t const* title)
    {
        // 客户端区域大小
        RECT rect = { 0, 0, width, height };
        // 根据客户区域宽和高计算整个窗口的宽和高
        if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
            CheckLastError();

        HWND hwnd = CreateWindowW(L"Default", title, WS_OVERLAPPEDWINDOW,
                               x, y, rect.right - rect.left, rect.bottom - rect.top,
                               nullptr, nullptr, nullptr, this);
        if (hwnd == nullptr)
            CheckLastError();
        // 显示窗口
        ShowWindow(hwnd, SW_SHOW);

        return hwnd;
    }

    void Win32PollEvents()
    {
        MSG msg = {};

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    std::pmr::string WindowMessageToString(UINT msg, WPARAM wp, LPARAM lp)
    {
        static std::unordered_map<DWORD, char const*> windowMessage
        {
            { WM_NULL,                  "WM_NULL" },
            { WM_CREATE,                "WM_CREATE" },
            { WM_DESTROY,               "WM_DESTROY" },
            { WM_MOVE,                  "WM_MOVE" },
            { WM_SIZE,                  "WM_SIZE" },
            { WM_ACTIVATE,              "WM_ACTIVATE" },
            { WM_SETFOCUS,              "WM_SETFOCUS" },
            { WM_KILLFOCUS,             "WM_KILLFOCUS" },
            { WM_ENABLE,                "WM_ENABLE" },
            { WM_SETREDRAW,             "WM_SETREDRAW" },
            { WM_SETTEXT,               "WM_SETTEXT" },
            { WM_GETTEXT,               "WM_GETTEXT" },
            { WM_GETTEXTLENGTH,         "WM_GETTEXTLENGTH" },
            { WM_PAINT,                 "WM_PAINT" },
            { WM_CLOSE,                 "WM_CLOSE" },
            { WM_QUERYENDSESSION,       "WM_QUERYENDSESSION" },
            { WM_QUIT,                  "WM_QUIT" },
            { WM_QUERYOPEN,             "WM_QUERYOPEN" },
            { WM_ERASEBKGND,            "WM_ERASEBKGND" },
            { WM_SYSCOLORCHANGE,        "WM_SYSCOLORCHANGE" },
            { WM_ENDSESSION,            "WM_ENDSESSION" },
            { WM_SHOWWINDOW,            "WM_SHOWWINDOW" },
            { WM_CTLCOLORMSGBOX,        "WM_CTLCOLORMSGBOX" },
            { WM_CTLCOLOREDIT,          "WM_CTLCOLOREDIT" },
            { WM_CTLCOLORLISTBOX,       "WM_CTLCOLORLISTBOX" },
            { WM_CTLCOLORBTN,           "WM_CTLCOLORBTN" },
            { WM_CTLCOLORDLG,           "WM_CTLCOLORDLG" },
            { WM_CTLCOLORSCROLLBAR,     "WM_CTLCOLORSCROLLBAR" },
            { WM_CTLCOLORSTATIC,        "WM_CTLCOLORSTATIC" },
            { WM_WININICHANGE,          "WM_WININICHANGE" },
            { WM_SETTINGCHANGE,         "WM_SETTINGCHANGE" },
            { WM_DEVMODECHANGE,         "WM_DEVMODECHANGE" },
            { WM_ACTIVATEAPP,           "WM_ACTIVATEAPP" },
            { WM_FONTCHANGE,            "WM_FONTCHANGE" },
            { WM_TIMECHANGE,            "WM_TIMECHANGE" },
            { WM_CANCELMODE,            "WM_CANCELMODE" },
            { WM_SETCURSOR,             "WM_SETCURSOR" },
            { WM_MOUSEACTIVATE,         "WM_MOUSEACTIVATE" },
            { WM_CHILDACTIVATE,         "WM_CHILDACTIVATE" },
            { WM_QUEUESYNC,             "WM_QUEUESYNC" },
            { WM_GETMINMAXINFO,         "WM_GETMINMAXINFO" },
            { WM_ICONERASEBKGND,        "WM_ICONERASEBKGND" },
            { WM_NEXTDLGCTL,            "WM_NEXTDLGCTL" },
            { WM_SPOOLERSTATUS,         "WM_SPOOLERSTATUS" },
            { WM_DRAWITEM,              "WM_DRAWITEM" },
            { WM_MEASUREITEM,           "WM_MEASUREITEM" },
            { WM_DELETEITEM,            "WM_DELETEITEM" },
            { WM_VKEYTOITEM,            "WM_VKEYTOITEM" },
            { WM_CHARTOITEM,            "WM_CHARTOITEM" },
            { WM_SETFONT,               "WM_SETFONT" },
            { WM_GETFONT,               "WM_GETFONT" },
            { WM_QUERYDRAGICON,         "WM_QUERYDRAGICON" },
            { WM_COMPAREITEM,           "WM_COMPAREITEM" },
            { WM_GETOBJECT,             "WM_GETOBJECT" },
            { WM_COMPACTING,            "WM_COMPACTING" },
            { WM_NCCREATE,              "WM_NCCREATE" },
            { WM_NCDESTROY,             "WM_NCDESTROY" },
            { WM_NCCALCSIZE,            "WM_NCCALCSIZE" },
            { WM_NCHITTEST,             "WM_NCHITTEST" },
            { WM_NCPAINT,               "WM_NCPAINT" },
            { WM_NCACTIVATE,            "WM_NCACTIVATE" },
            { WM_GETDLGCODE,            "WM_GETDLGCODE" },
            { WM_NCMOUSEMOVE,           "WM_NCMOUSEMOVE" },
            { WM_NCLBUTTONDOWN,         "WM_NCLBUTTONDOWN" },
            { WM_NCLBUTTONUP,           "WM_NCLBUTTONUP" },
            { WM_NCLBUTTONDBLCLK,       "WM_NCLBUTTONDBLCLK" },
            { WM_NCRBUTTONDOWN,         "WM_NCRBUTTONDOWN" },
            { WM_NCRBUTTONUP,           "WM_NCRBUTTONUP" },
            { WM_NCRBUTTONDBLCLK,       "WM_NCRBUTTONDBLCLK" },
            { WM_NCMBUTTONDOWN,         "WM_NCRBUTTONDBLCLK" },
            { WM_NCMBUTTONUP,           "WM_NCMBUTTONUP" },
            { WM_NCMBUTTONDBLCLK,       "WM_NCMBUTTONDBLCLK" },
            { WM_KEYDOWN,               "WM_KEYDOWN" },
            { WM_KEYUP,                 "WM_KEYUP" },
            { WM_CHAR,                  "WM_CHAR" },
            { WM_DEADCHAR,              "WM_DEADCHAR" },
            { WM_SYSKEYDOWN,            "WM_SYSKEYDOWN" },
            { WM_SYSKEYUP,              "WM_SYSKEYUP" },
            { WM_SYSCHAR,               "WM_SYSCHAR" },
            { WM_SYSDEADCHAR,           "WM_SYSDEADCHAR" },
            { WM_KEYLAST,               "WM_KEYLAST" },
            { WM_INITDIALOG,            "WM_INITDIALOG" },
            { WM_COMMAND,               "WM_COMMAND" },
            { WM_SYSCOMMAND,            "WM_SYSCOMMAND" },
            { WM_TIMER,                 "WM_TIMER" },
            { WM_HSCROLL,               "WM_HSCROLL" },
            { WM_VSCROLL,               "WM_VSCROLL" },
            { WM_INITMENU,              "WM_INITMENU" },
            { WM_INITMENUPOPUP,         "WM_INITMENUPOPUP" },
            { WM_MENUSELECT,            "WM_MENUSELECT" },
            { WM_MENUCHAR,              "WM_MENUCHAR" },
            { WM_ENTERIDLE,             "WM_ENTERIDLE" },
            { WM_MOUSEWHEEL,            "WM_MOUSEWHEEL" },
            { WM_MOUSEMOVE,             "WM_MOUSEMOVE" },
            { WM_LBUTTONDOWN,           "WM_LBUTTONDOWN" },
            { WM_LBUTTONUP,             "WM_LBUTTONUP" },
            { WM_LBUTTONDBLCLK,         "WM_LBUTTONDBLCLK" },
            { WM_RBUTTONDOWN,           "WM_RBUTTONDOWN" },
            { WM_RBUTTONUP,             "WM_RBUTTONUP" },
            { WM_RBUTTONDBLCLK,         "WM_RBUTTONDBLCLK" },
            { WM_MBUTTONDOWN,           "WM_MBUTTONDOWN" },
            { WM_MBUTTONUP,             "WM_MBUTTONUP" },
            { WM_MBUTTONDBLCLK,         "WM_MBUTTONDBLCLK" },
            { WM_PARENTNOTIFY,          "WM_PARENTNOTIFY" },
            { WM_MDICREATE,             "WM_MDICREATE" },
            { WM_MDIDESTROY,            "WM_MDIDESTROY" },
            { WM_MDIACTIVATE,           "WM_MDIACTIVATE" },
            { WM_MDIRESTORE,            "WM_MDIRESTORE" },
            { WM_MDINEXT,               "WM_MDINEXT" },
            { WM_MDIMAXIMIZE,           "WM_MDIMAXIMIZE" },
            { WM_MDITILE,               "WM_MDITILE" },
            { WM_MDICASCADE,            "WM_MDICASCADE" },
            { WM_MDIICONARRANGE,        "WM_MDIICONARRANGE" },
            { WM_MDIGETACTIVE,          "WM_MDIGETACTIVE" },
            { WM_MDISETMENU,            "WM_MDISETMENU" },
            { WM_CUT,                   "WM_CUT" },
            { WM_COPYDATA,              "WM_COPYDATA" },
            { WM_COPY,                  "WM_COPY" },
            { WM_PASTE,                 "WM_PASTE" },
            { WM_CLEAR,                 "WM_CLEAR" },
            { WM_UNDO,                  "WM_UNDO" },
            { WM_RENDERFORMAT,          "WM_RENDERFORMAT" },
            { WM_RENDERALLFORMATS,      "WM_RENDERALLFORMATS" },
            { WM_DESTROYCLIPBOARD,      "WM_DESTROYCLIPBOARD" },
            { WM_DRAWCLIPBOARD,         "WM_DRAWCLIPBOARD" },
            { WM_PAINTCLIPBOARD,        "WM_PAINTCLIPBOARD" },
            { WM_VSCROLLCLIPBOARD,      "WM_VSCROLLCLIPBOARD" },
            { WM_SIZECLIPBOARD,         "WM_SIZECLIPBOARD" },
            { WM_ASKCBFORMATNAME,       "WM_ASKCBFORMATNAME" },
            { WM_CHANGECBCHAIN,         "WM_CHANGECBCHAIN" },
            { WM_HSCROLLCLIPBOARD,      "WM_HSCROLLCLIPBOARD" },
            { WM_QUERYNEWPALETTE,       "WM_QUERYNEWPALETTE"},
            { WM_PALETTEISCHANGING,     "WM_PALETTEISCHANGING" },
            { WM_PALETTECHANGED,        "WM_PALETTECHANGED" },
            { WM_DROPFILES,             "WM_DROPFILES" },
            { WM_POWER,                 "WM_POWER" },
            { WM_WINDOWPOSCHANGED,      "WM_WINDOWPOSCHANGED" },
            { WM_WINDOWPOSCHANGING,     "WM_WINDOWPOSCHANGING" },
            { WM_HELP,                  "WM_HELP" },
            { WM_NOTIFY,                "WM_NOTIFY" },
            { WM_CONTEXTMENU,           "WM_CONTEXTMENU" },
            { WM_TCARD,                 "WM_TCARD" },
            { WM_MDIREFRESHMENU,        "WM_MDIREFRESHMENU" },
            { WM_MOVING,                "WM_MOVING" },
            { WM_STYLECHANGED,          "WM_STYLECHANGED" },
            { WM_STYLECHANGING,         "WM_STYLECHANGING" },
            { WM_SIZING,                "WM_SIZING" },
            { WM_SETHOTKEY,             "WM_SETHOTKEY" },
            { WM_PRINT,                 "WM_PRINT" },
            { WM_PRINTCLIENT,           "WM_PRINTCLIENT" },
            { WM_POWERBROADCAST,        "WM_POWERBROADCAST" },
            { WM_HOTKEY,                "WM_HOTKEY" },
            { WM_GETICON,               "WM_GETICON" },
            { WM_EXITMENULOOP,          "WM_EXITMENULOOP" },
            { WM_ENTERMENULOOP,         "WM_ENTERMENULOOP" },
            { WM_DISPLAYCHANGE,         "WM_DISPLAYCHANGE" },
            { WM_STYLECHANGED,          "WM_STYLECHANGED" },
            { WM_STYLECHANGING,         "WM_STYLECHANGING" },
            { WM_GETICON,               "WM_GETICON" },
            { WM_SETICON,               "WM_SETICON" },
            { WM_SIZING,                "WM_SIZING" },
            { WM_MOVING,                "WM_MOVING" },
            { WM_CAPTURECHANGED,        "WM_CAPTURECHANGED" },
            { WM_DEVICECHANGE,          "WM_DEVICECHANGE" },
            { WM_PRINT,                 "WM_PRINT" },
            { WM_PRINTCLIENT,           "WM_PRINTCLIENT" },
            { WM_IME_SETCONTEXT,        "WM_IME_SETCONTEXT" },
            { WM_IME_NOTIFY,            "WM_IME_NOTIFY" },
            { WM_NCMOUSELEAVE,          "WM_NCMOUSELEAVE" },
            { WM_EXITSIZEMOVE,          "WM_EXITSIZEMOVE" },
            { WM_DWMNCRENDERINGCHANGED, "WM_DWMNCRENDERINGCHANGED" },
            { WM_ENTERSIZEMOVE,         "WM_ENTERSIZEMOVE" },
        };

        const auto it = windowMessage.find(msg);
        std::pmr::string msgName;
        if (it == windowMessage.end())
            std::format_to(std::back_inserter(msgName), "Unknown message: {:#x}", msg);
        else
            msgName.append(it->second);

        std::pmr::string msgStr;
        std::format_to(std::back_inserter(msgStr), "{:<25} LP: {:#018x}   WP: {:#018x}", msgName, lp, wp);

        return msgStr;
    }

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

    // 注册窗口类
    static char RegisterWindow()
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

        return {};
    }

    // 在main之前调用RegisterWindow函数
    [[maybe_unused]] static char gPlaceHolder = RegisterWindow();
}
