/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/EventDispatcher.h"

namespace TG::Input
{
    void EventDispatcher::Dispatch(const Event& event) const
    {
        for (std::reference_wrapper<IEventHandler> handler: m_handlers)
            handler.get().Consume(event);
    }

    void EventDispatcher::Register(IEventHandler& handler)
    {
        m_handlers.emplace_back(handler);
    }

    void EventDispatcher::UnRegister(const IEventHandler& handler)
    {
        std::erase_if(m_handlers,
            [&handler](const std::reference_wrapper<IEventHandler> refWrapper)
            {
                return std::addressof(refWrapper.get()) == std::addressof(handler);
            });
    }

    KeyCode ToKeyCode(PAL::Key key)
    {
        switch (key)
        {
            case PAL::Key::Backspace:
                return KeyCode::Backspace;
            case PAL::Key::Tab:
                return KeyCode::Tab;
            case PAL::Key::Enter:
                return KeyCode::Enter;
            case PAL::Key::Esc:
                return KeyCode::Esc;
            case PAL::Key::Space:
                return KeyCode::Space;
            case PAL::Key::PageUp:
                return KeyCode::PageUp;
            case PAL::Key::PageDown:
                return KeyCode::PageDown;
            case PAL::Key::End:
                return KeyCode::End;
            case PAL::Key::Home:
                return KeyCode::Home;
            case PAL::Key::LeftArrow:
                return KeyCode::LeftArrow;
            case PAL::Key::UpArrow:
                return KeyCode::UpArrow;
            case PAL::Key::RightArrow:
                return KeyCode::RightArrow;
            case PAL::Key::DownArrow:
                return KeyCode::DownArrow;
            case PAL::Key::Insert:
                return KeyCode::Insert;
            case PAL::Key::Delete:
                return KeyCode::Delete;
            case PAL::Key::Num0:
                return KeyCode::Num0;
            case PAL::Key::Num1:
                return KeyCode::Num1;
            case PAL::Key::Num2:
                return KeyCode::Num2;
            case PAL::Key::Num3:
                return KeyCode::Num3;
            case PAL::Key::Num4:
                return KeyCode::Num4;
            case PAL::Key::Num5:
                return KeyCode::Num5;
            case PAL::Key::Num6:
                return KeyCode::Num6;
            case PAL::Key::Num7:
                return KeyCode::Num7;
            case PAL::Key::Num8:
                return KeyCode::Num8;
            case PAL::Key::Num9:
                return KeyCode::Num9;
            case PAL::Key::A:
                return KeyCode::A;
            case PAL::Key::B:
                return KeyCode::B;
            case PAL::Key::C:
                return KeyCode::C;
            case PAL::Key::D:
                return KeyCode::D;
            case PAL::Key::E:
                return KeyCode::E;
            case PAL::Key::F:
                return KeyCode::F;
            case PAL::Key::G:
                return KeyCode::G;
            case PAL::Key::H:
                return KeyCode::H;
            case PAL::Key::I:
                return KeyCode::I;
            case PAL::Key::J:
                return KeyCode::J;
            case PAL::Key::K:
                return KeyCode::K;
            case PAL::Key::L:
                return KeyCode::L;
            case PAL::Key::M:
                return KeyCode::M;
            case PAL::Key::N:
                return KeyCode::N;
            case PAL::Key::O:
                return KeyCode::O;
            case PAL::Key::P:
                return KeyCode::P;
            case PAL::Key::Q:
                return KeyCode::Q;
            case PAL::Key::R:
                return KeyCode::R;
            case PAL::Key::S:
                return KeyCode::S;
            case PAL::Key::T:
                return KeyCode::T;
            case PAL::Key::U:
                return KeyCode::U;
            case PAL::Key::V:
                return KeyCode::V;
            case PAL::Key::W:
                return KeyCode::W;
            case PAL::Key::X:
                return KeyCode::X;
            case PAL::Key::Y:
                return KeyCode::Y;
            case PAL::Key::Z:
                return KeyCode::Z;
            case PAL::Key::Keypad0:
                return KeyCode::Keypad0;
            case PAL::Key::Keypad1:
                return KeyCode::Keypad1;
            case PAL::Key::Keypad2:
                return KeyCode::Keypad2;
            case PAL::Key::Keypad3:
                return KeyCode::Keypad3;
            case PAL::Key::Keypad4:
                return KeyCode::Keypad4;
            case PAL::Key::Keypad5:
                return KeyCode::Keypad5;
            case PAL::Key::Keypad6:
                return KeyCode::Keypad6;
            case PAL::Key::Keypad7:
                return KeyCode::Keypad7;
            case PAL::Key::Keypad8:
                return KeyCode::Keypad8;
            case PAL::Key::Keypad9:
                return KeyCode::Keypad9;
            case PAL::Key::KeypadMutiply:
                return KeyCode::KeypadMutiply;
            case PAL::Key::KeypadAdd:
                return KeyCode::KeypadAdd;
            case PAL::Key::KeypadSubtract:
                return KeyCode::KeypadSubtract;
            case PAL::Key::KeypadPeriod:
                return KeyCode::KeypadPeriod;
            case PAL::Key::KeypadDivide:
                return KeyCode::KeypadDivide;
            case PAL::Key::F1:
                return KeyCode::F1;
            case PAL::Key::F2:
                return KeyCode::F2;
            case PAL::Key::F3:
                return KeyCode::F3;
            case PAL::Key::F4:
                return KeyCode::F4;
            case PAL::Key::F5:
                return KeyCode::F5;
            case PAL::Key::F6:
                return KeyCode::F6;
            case PAL::Key::F7:
                return KeyCode::F7;
            case PAL::Key::F8:
                return KeyCode::F8;
            case PAL::Key::F9:
                return KeyCode::F9;
            case PAL::Key::F10:
                return KeyCode::F10;
            case PAL::Key::F11:
                return KeyCode::F11;
            case PAL::Key::F12:
                return KeyCode::F12;
            case PAL::Key::LeftShift:
                return KeyCode::LeftShift;
            case PAL::Key::RightShift:
                return KeyCode::RightShift;
            case PAL::Key::LeftControl:
                return KeyCode::LeftControl;
            case PAL::Key::RightControl:
                return KeyCode::RightControl;
            case PAL::Key::LeftAlt:
                return KeyCode::LeftAlt;
            case PAL::Key::RightAlt:
                return KeyCode::RightAlt;
            case PAL::Key::Semicolon:
                return KeyCode::Semicolon;
            case PAL::Key::Comma:
                return KeyCode::Comma;
            case PAL::Key::Add:
                return KeyCode::Add;
            case PAL::Key::Subtract:
                return KeyCode::Subtract;
            case PAL::Key::Period:
                return KeyCode::Period;
            case PAL::Key::Slash:
                return KeyCode::Slash;
            case PAL::Key::BackQuote:
                return KeyCode::BackQuote;
            case PAL::Key::LeftBracket:
                return KeyCode::LeftBracket;
            case PAL::Key::BackSlash:
                return KeyCode::BackSlash;
            case PAL::Key::RightBracket:
                return KeyCode::RightBracket;
            case PAL::Key::Quote:
                return KeyCode::Quote;
            default:
                return KeyCode::None;
        }
    }

    KeyCode ToKeyCode(PAL::MouseButton button)
    {
        switch (button)
        {
            case PAL::MouseButton::Left:
                return KeyCode::LeftMouseButton;
            case PAL::MouseButton::Middle:
                return KeyCode::MiddleMouseButton;
            case PAL::MouseButton::Right:
                return KeyCode::RightMouseButton;
            default:
                return KeyCode::None;
        }
    }
}
