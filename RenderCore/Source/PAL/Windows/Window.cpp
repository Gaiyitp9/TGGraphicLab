/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Window.h"
#include "PAL/Windows/NativeWindow.h"
#include "PAL/Windows/Win32Exception.h"
#include "PAL/Windows/Auxiliary.h"

namespace TG::PAL
{
	Window::Window(int x, int y, int width, int height, std::string_view name, WindowType type)
		: m_nativeWindow(std::make_unique<NativeWindow>(std::string(name)))
	{
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		DWORD dwExStyle = WS_EX_OVERLAPPEDWINDOW;
		switch (type)
		{
		case WindowType::Default:
			dwStyle = WS_OVERLAPPEDWINDOW;
			dwExStyle = WS_EX_OVERLAPPEDWINDOW;
			break;
		case WindowType::Load:
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_TOOLWINDOW;
			dwExStyle &= ~WS_EX_APPWINDOW;
			break;
		}

		// 客户端区域大小
		RECT rect = { 0, 0, width, height };
		// 根据客户区域宽和高计算整个窗口的宽和高
		if (!AdjustWindowRect(&rect, dwStyle, false))
			CheckLastError();
		m_nativeWindow->hwnd = CreateWindowExW(dwExStyle, L"Default", Utf8ToUtf16(name).c_str(), dwStyle,
							   x, y, rect.right - rect.left, rect.bottom - rect.top,
							   nullptr, nullptr, nullptr, m_nativeWindow.get());
		if (m_nativeWindow->hwnd == nullptr)
			CheckLastError();

		// 显示窗口
		ShowWindow(m_nativeWindow->hwnd, SW_SHOW);
	}

	// 需要在这里定义析构函数，防止其变成内联函数，让std::unique_ptr<NativeWindow>能通过编译
	Window::~Window() = default;

	void Window::SetKeyCallback(const KeyFunction& function) const
	{
		m_nativeWindow->keyFunction = function;
	}

	bool Window::IsDestroyed() const
	{
		return m_nativeWindow->destroyed;
	}

	void Window::SetIcon(std::string_view iconPath) const
	{
		HANDLE icon = LoadImageW(nullptr, Utf8ToUtf16(iconPath).c_str(), IMAGE_ICON, 0, 0,
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (icon == nullptr)
			CheckLastError("Invalid icon source");

		SendMessageW(m_nativeWindow->hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessageW(m_nativeWindow->hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}

	void Window::SetCharCallback(const CharFunction &function) const
	{
		m_nativeWindow->charFunction = function;
	}

	void Window::SetMouseButtonCallback(const MouseButtonFunction &function) const
	{
		m_nativeWindow->mouseButtonFunction = function;
	}

	void Window::SetCursorPosCallback(const CursorPosFunction &function) const
	{
		m_nativeWindow->cursorPosFunction = function;
	}

	void Window::SetScrollCallback(const ScrollFunction &function) const
	{
		m_nativeWindow->scrollFunction = function;
	}

	void Window::SetWindowPosCallback(const WindowPosFunction &function) const
	{
		m_nativeWindow->windowPosFunction = function;
	}

	void Window::SetWindowSizeCallback(const WindowSizeFunction &function) const
	{
		m_nativeWindow->windowSizeFunction = function;
	}

	void Window::SetSuspendCallback(const SuspendFunction &function) const
	{
		m_nativeWindow->suspendFunction = function;
	}

	void Window::SetResumeCallback(const ResumeFunction &function) const
	{
		m_nativeWindow->resumeFunction = function;
	}

    // 轮询输入事件
    std::optional<int> PollEvents()
    {
		MSG msg = { nullptr };

		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return static_cast<int>(msg.wParam);

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return std::nullopt;
    }

    // 窗口消息转成字符串
    static std::pmr::string WindowMessageToString(UINT msg, WPARAM wp, LPARAM lp)
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

        std::pmr::string message;
        std::format_to(std::back_inserter(message), "{:<25} LP: {:#018x}   WP: {:#018x}", msgName, lp, wp);

        return message;
    }

    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
        // 注：窗口处理函数不能向上传递异常
        auto* const pWindow = reinterpret_cast<NativeWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

		// 监控窗口消息
		if (pWindow->spyMessage)
		{
		    std::pmr::string windowMessage;
		    std::format_to(std::back_inserter(windowMessage), "{:<16} {}\n", pWindow->name, WindowMessageToString(msg, wParam, lParam));
			OutputDebugStringA(windowMessage.data());
		}

		switch (msg)
		{
		case WM_DESTROY:
		{
            // 窗口被销毁后，窗口类也需要被销毁
            pWindow->destroyed = true;
			// 基础窗口一般作为主窗口，销毁后要退出线程
			PostQuitMessage(0);
			return 0;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
		{
            // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input
            WORD vkCode = LOWORD(wParam);
            WORD keyFlags = HIWORD(lParam);
            WORD scanCode = LOBYTE(keyFlags);
			// extended-key flag, 1 if scancode has 0xE0 prefix
            if (keyFlags & KF_EXTENDED == KF_EXTENDED)
                scanCode = MAKEWORD(scanCode, 0xE0);
            switch (vkCode)
            {
                case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
                case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
                case VK_MENU:    // converts to VK_LMENU or VK_RMENU
                    vkCode = LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
                    break;

                default:
                    break;
            }
			// 确定按键状态，按下、释放还是按住
			InputAction action = InputAction::Press;
			if (keyFlags & KF_UP == KF_UP)
				action = InputAction::Release;
			else if (keyFlags & KF_REPEAT == KF_REPEAT)
				action = InputAction::Repeat;

			if (pWindow->keyFunction)
				pWindow->keyFunction(static_cast<Key>(vkCode), scanCode, action);

			return 0;
		}

		case WM_CHAR:
        {
            if (pWindow->charFunction)
            	pWindow->charFunction(wParam);
			return 0;
        }

		case WM_MOUSEMOVE:
		{
			if (pWindow->cursorPosFunction)
				pWindow->cursorPosFunction(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Left, InputAction::Press);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Left, InputAction::Release);
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Right, InputAction::Press);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Right, InputAction::Release);
			return 0;
		}

		case WM_MBUTTONDOWN:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Middle, InputAction::Press);
			return 0;
		}

		case WM_MBUTTONUP:
		{
			if (pWindow->mouseButtonFunction)
				pWindow->mouseButtonFunction(MouseButton::Middle, InputAction::Release);
			return 0;
		}

		case WM_MOUSEWHEEL:
        {
			// 每帧只会产生一个WM_MOUSEWHEEL
			if (pWindow->scrollFunction)
				pWindow->scrollFunction(0, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			return 0;
        }

		case WM_MOVE:
		{
			if (pWindow->windowPosFunction)
				pWindow->windowPosFunction(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		case WM_SIZE:
		{
			if (pWindow->windowSizeFunction)
				pWindow->windowSizeFunction(LOWORD(lParam), HIWORD(lParam));
			if (pWindow->suspendFunction && wParam == SIZE_MINIMIZED)
				pWindow->suspendFunction();
			if (pWindow->resumeFunction && wParam == SIZE_RESTORED)
				pWindow->resumeFunction();
			return 0;
		}

		case WM_ENTERSIZEMOVE:
            if (pWindow->suspendFunction)
                pWindow->suspendFunction();
			return 0;

		case WM_EXITSIZEMOVE:
            if (pWindow->resumeFunction)
                pWindow->resumeFunction();
			return 0;

        default:
		    return DefWindowProcW(hwnd, msg, wParam, lParam);
		}
	}

    static LRESULT WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // 注：窗口处理函数不能向上传递异常
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* pWnd = static_cast<NativeWindow*>(pCreate->lpCreateParams);

            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));

            return WindowProc(hwnd, msg, wParam, lParam);
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
