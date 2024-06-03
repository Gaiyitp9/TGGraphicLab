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
