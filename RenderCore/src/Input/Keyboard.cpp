/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Keyboard.hpp"
#include "Diagnostics/Log.hpp"
#include "Input/EventData.hpp"
#include <format>
#include <cassert>

namespace TG::Input
{
	Keyboard::Keyboard() = default;

	Keyboard::~Keyboard() = default;

	void Keyboard::PreUpdate()
	{
		m_keyDown.reset();
		m_keyUp.reset();
	}

    void Keyboard::Receive(const Event &e)
    {
        auto key = static_cast<size_t>(e.key);
        switch (e.type)
        {
            case EventType::Press:
                // 第一次按下，key down为true
                if (!m_keyHold.test(key))
                    m_keyDown[key] = true;
                m_keyHold[key] = true;
                break;

            case EventType::Release:
                // 放开按键，key up为true(不存在第一次放开，每一次WM_KEYUP都是第一次放开)
                m_keyUp[key] = true;
                m_keyHold[key] = false;
                break;

            case EventType::Char:
            {
                auto data = std::any_cast<KeyboardData>(&e.data);
                assert(data && "Char event doesn't contain data or the data type is not KeyboradData");
                if (std::isprint(data->c))
                    Debug::LogLine(data->c);
                break;
            }

            default:
                break;
        }

        if (m_spyKeyboard && EventInfo::keysName.contains(e.key) && EventInfo::eventTypes.contains(e.type))
        {
            Debug::LogLine(std::format("Key: {:<20} Event: {:<20} ", EventInfo::keysName.at(e.key), EventInfo::eventTypes.at(e.type)));
        }
    }

    void Keyboard::SpyEvent(bool enable)
    {
        m_spyKeyboard = enable;
    }

    bool Keyboard::GetKey(KeyCode k)
    {
        auto pos = static_cast<size_t>(k);
        return m_keyHold.test(pos);
    }

    bool Keyboard::GetKeyDown(KeyCode k)
    {
        auto pos = static_cast<size_t>(k);
        return m_keyDown.test(pos);
    }

    bool Keyboard::GetKeyUp(KeyCode k)
    {
        auto pos = static_cast<size_t>(k);
        return m_keyUp.test(pos);
    }
}