/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Windows/Win32Window.h"
#include "Exception/Windows/Win32Exception.h"
#include "Base/Utility.h"
// #include <hidusage.h>
#include <format>

namespace TG
{
	Win32Window::Win32Window(int x, int y, int width, int height, std::string_view name, WindowType type)
		: m_name(name)
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
			throw Win32Exception::Create();
		m_handle = CreateWindowExW(dwExStyle, L"Default", MultiBytesToWideChars(name).c_str(), dwStyle,
							   x, y, rect.right - rect.left, rect.bottom - rect.top,
							   nullptr, nullptr, nullptr, this);
		if (m_handle == nullptr)
			throw Win32Exception::Create();

		m_deviceContext = GetDC(m_handle);

		// RAWINPUTDEVICE rid[2];
		// // 鼠标
		// rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		// rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		// rid[0].dwFlags = 0;
		// rid[0].hwndTarget = m_handle;
		// // 键盘
		// rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		// rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
		// rid[1].dwFlags = 0;
		// rid[1].hwndTarget = m_handle;
		// // 注册输入设备
		// if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
		// 	throw Win32Exception::Create("Register mouse and keyboard failed");
	}

	Win32Window::~Win32Window()
	{
		ReleaseDC(m_handle, m_deviceContext);
	}

	std::optional<int> Win32Window::PollEvents()
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

	void Win32Window::SetIcon(std::string_view iconPath) const
	{
		HANDLE icon = LoadImageW(nullptr, MultiBytesToWideChars(iconPath).c_str(), IMAGE_ICON, 0, 0,
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (icon == nullptr)
			throw Win32Exception::Create("Invalid icon source");

		SendMessageW(m_handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessageW(m_handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}

	void Win32Window::SetPosition(int x, int y) const
	{
		SetWindowPos(m_handle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}

	void Win32Window::SetSize(unsigned int width, unsigned int height) const
	{
		SetWindowPos(m_handle, HWND_TOP, 0, 0, static_cast<int>(width), static_cast<int>(height), SWP_NOMOVE);
	}

	void Win32Window::Show(bool show) const
	{
		ShowWindow(m_handle, show ? SW_SHOW : SW_HIDE);
	}

    // 窗口消息转成字符串
    static std::string WindowMessageToString(UINT msg, WPARAM wp, LPARAM lp)
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
            { WM_IME_STARTCOMPOSITION,  "WM_IME_STARTCOMPOSITION" },
        	{ WM_IME_COMPOSITION,		"WM_IME_COMPOSITION"},
        	{ WM_IME_ENDCOMPOSITION,	"WM_IME_ENDCOMPOSITION"},
            { WM_IME_SELECT,			"WM_IME_SELECT" },
        	{ WM_IME_CHAR,				"WM_IME_CHAR" },
        	{ WM_IME_REQUEST,			"WM_IME_REQUEST" },
        	{ WM_IME_KEYDOWN,			"WM_IME_KEYDOWN" },
        	{ WM_IME_KEYUP,				"WM_IME_UP" },
            { WM_NCMOUSELEAVE,          "WM_NCMOUSELEAVE" },
            { WM_EXITSIZEMOVE,          "WM_EXITSIZEMOVE" },
            { WM_DWMNCRENDERINGCHANGED, "WM_DWMNCRENDERINGCHANGED" },
            { WM_ENTERSIZEMOVE,         "WM_ENTERSIZEMOVE" },
        	{ WM_INPUT,					"WM_INPUT" },
        };

        const auto it = windowMessage.find(msg);
        std::string msgName;
        if (it == windowMessage.end())
            std::format_to(std::back_inserter(msgName), "Unknown message: {:#x}", msg);
        else
            msgName.append(it->second);

        std::string message;
        std::format_to(std::back_inserter(message), "{:<25} LP: {:#018x}   WP: {:#018x}", msgName, lp, wp);

        return message;
    }

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
        // 注：窗口处理函数不能向上传递异常
        auto* const pWindow = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

		// 监控窗口消息
		// 因为打印窗口消息需要频繁创建string，小字符串优化(SSO)不适用，所以最好使用内存池避免频繁分配内存
	    std::string windowMessage;
	    std::format_to(std::back_inserter(windowMessage), "{:<16} {}\n", pWindow->m_name,
	    	WindowMessageToString(msg, wParam, lParam));
		OutputDebugStringA(windowMessage.data());

		switch (msg)
		{
			case WM_DESTROY:
			{
	            // 窗口被销毁后，窗口类也需要被销毁
	            pWindow->m_destroyed = true;
				// 基础窗口一般作为主窗口，销毁后要退出线程
				PostQuitMessage(0);
				return 0;
			}

			// case WM_INPUT:
			// {
			// 	UINT dwSize = sizeof(RAWINPUT);
			// 	BYTE lpb[sizeof(RAWINPUT)];
			// 	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, lpb,
			// 		&dwSize, sizeof(RAWINPUTHEADER)) == -1)
			// 	{
			// 		break;
			// 	}
			//
			// 	if (auto* rawInput = reinterpret_cast<RAWINPUT*>(lpb); rawInput->header.dwType == RIM_TYPEMOUSE)
			// 	{
			// 		if (const RAWMOUSE& mouse = rawInput->data.mouse; mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
			// 		{
			// 			RECT rect;
			// 			if (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
			// 			{
			// 				rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
			// 				rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
			// 				rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			// 				rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
			// 			}
			// 			else
			// 			{
			// 				rect.left = 0;
			// 				rect.top = 0;
			// 				rect.right = GetSystemMetrics(SM_CXSCREEN);
			// 				rect.bottom = GetSystemMetrics(SM_CYSCREEN);
			// 			}
			//
			// 			POINT screenPos;
			// 			screenPos.x = MulDiv(mouse.lLastX, rect.right, USHRT_MAX) + rect.left;
			// 			screenPos.y = MulDiv(mouse.lLastY, rect.bottom, USHRT_MAX) + rect.top;
			// 			ScreenToClient(pWindow->m_handle, &screenPos);
			// 			pWindow->m_cursorPosDelegate.ExecuteIfBound(screenPos.x, screenPos.y);
			// 		}
			// 		else if (mouse.lLastX != 0 || mouse.lLastY != 0)
			// 		{
			// 			int relativeX = mouse.lLastX;
			// 			int relativeY = mouse.lLastY;
			// 			// 更新鼠标位置
			// 			POINT screenPos;
			// 			GetCursorPos(&screenPos);
			// 			ScreenToClient(pWindow->m_handle, &screenPos);
			// 			pWindow->m_cursorPosDelegate.ExecuteIfBound(screenPos.x, screenPos.y);
			// 			OutputDebugStringA(std::to_string(screenPos.x).c_str());
			// 			OutputDebugStringA(std::to_string(screenPos.y).c_str());
			// 		}
			//
			// 		// 鼠标按钮状态
			// 		USHORT buttonFlags = rawInput->data.mouse.usButtonFlags;
			// 		if (buttonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
			// 			// 左键按下
			// 			OutputDebugStringA("left Mouse down\n");
			// 		}
			// 		if (buttonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
			// 			// 左键释放
			// 			OutputDebugStringA("left Mouse up\n");
			// 		}
			// 		// 类似处理中键、右键等
			// 	}
			// 	else if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
			// 	{
			// 		RAWKEYBOARD kb = rawInput->data.keyboard;
			// 		// 虚拟键码
			// 		USHORT vKey = kb.VKey;
			// 		// 按下或释放
			// 		bool isKeyDown = !(kb.Flags & RI_KEY_BREAK); // RI_KEY_MAKE表示按下
			//
			// 		// 示例：处理空格键
			// 		if (vKey == VK_SPACE && isKeyDown) {
			// 			// 空格键按下
			// 			OutputDebugStringA("Keyboard space down\n");
			// 		}
			// 	}
			//
			// 	return 0;
			// }

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
	            if ((keyFlags & KF_EXTENDED) == KF_EXTENDED)
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
				Input::Action action = Input::Action::Press;
				if ((keyFlags & KF_UP) == KF_UP)
					action = Input::Action::Release;
				else if ((keyFlags & KF_REPEAT) == KF_REPEAT)
					action = Input::Action::Repeat;

				pWindow->m_keyDelegate.ExecuteIfBound(static_cast<Input::KeyCode>(vkCode), scanCode, action);

				return 0;
			}

			case WM_CHAR:
	        {
	            pWindow->m_charDelegate.ExecuteIfBound(static_cast<char16_t>(wParam));
				return 0;
	        }

			case WM_MOUSEMOVE:
			{
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::LeftMouseButton, Input::Action::Press);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_LBUTTONUP:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::LeftMouseButton, Input::Action::Release);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::RightMouseButton, Input::Action::Press);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_RBUTTONUP:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::RightMouseButton, Input::Action::Release);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_MBUTTONDOWN:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::MiddleMouseButton, Input::Action::Press);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_MBUTTONUP:
			{
				pWindow->m_mouseButtonDelegate.ExecuteIfBound(Input::KeyCode::MiddleMouseButton, Input::Action::Release);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}

			case WM_MOUSEWHEEL:
	        {
				// 每帧只会产生一个WM_MOUSEWHEEL
				pWindow->m_scrollDelegate.ExecuteIfBound(0, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
				pWindow->m_cursorPosDelegate.ExecuteIfBound(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
	        }

			case WM_MOVE:
			{
				pWindow->m_windowPosDelegate.ExecuteIfBound(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}

			case WM_SIZE:
			{
				pWindow->m_windowSizeDelegate.ExecuteIfBound(LOWORD(lParam), HIWORD(lParam));
				if (wParam == SIZE_MINIMIZED)
					pWindow->m_suspendDelegate.ExecuteIfBound();
				if (wParam == SIZE_RESTORED)
					pWindow->m_resumeDelegate.ExecuteIfBound();
				return 0;
			}

			case WM_ENTERSIZEMOVE:
			{
				// 准备移动或改变窗口尺寸时，收到该消息
				// 但是窗口此时会冻结，添加Timer定时发送WM_TIMER消息，
				// 在WM_TIMER消息内添加需要更新的函数，比如渲染
				// SetTimer(hwnd, 1, 16, nullptr);
	            pWindow->m_suspendDelegate.ExecuteIfBound();
				return 0;
			}

			case WM_EXITSIZEMOVE:
			{
				// KillTimer(hwnd, 1);
	            pWindow->m_resumeDelegate.ExecuteIfBound();
				return 0;
			}

			// TODO: 添加窗口冻结时需要运行的逻辑
			// case WM_TIMER:
				// LogInfo("WM_TIMER");
				// return 0;

	        default:
	        	break;
		}
    	return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

    static LRESULT WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // 注：窗口处理函数不能向上传递异常
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* pWnd = static_cast<Win32Window*>(pCreate->lpCreateParams);

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
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WindowProcSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = nullptr;
        wc.hIcon = nullptr;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wc.hIconSm = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = L"Default";
        if (RegisterClassExW(&wc) == 0)
            throw Win32Exception::Create();

        return {};
    }

    // 在main之前调用RegisterWindow函数
    [[maybe_unused]] static char g_placeHolder = RegisterWindow();
}
