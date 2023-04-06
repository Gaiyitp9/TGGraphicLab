/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG::Input
{
	Mouse::Mouse() = default;

	Mouse::~Mouse() = default;

	void Mouse::Update()
	{
		mouseDown.reset();
		mouseUp.reset();
        m_wheelDelta = 0;
	}

    void Mouse::Receive(const Event& e)
    {
        auto key = static_cast<size_t>(e.key);
        switch (e.type)
        {
            case EventType::Press:
                mouseDown[key] = true;
                mouseHold[key] = true;
                break;
            case EventType::Release:
                mouseUp[key] = true;
                mouseHold[key] = false;
                break;
            case EventType::MouseMove:
            {
                if (std::holds_alternative<MouseData>(e.data))
                {
                    auto const data = std::get<MouseData>(e.data);
                    m_position.x() = data.x;
                    m_position.y() = data.y;
                }
                break;
            }

            case EventType::WheelRoll:
            {
                if (std::holds_alternative<MouseData>(e.data))
                    m_wheelDelta = std::get<MouseData>(e.data).delta;
                break;
            }
            default:
                break;
        }

        // 监控鼠标
        if (m_spyMouse)
            SpyMouseEvent(e);
    }

    void Mouse::SpyEvent(bool enable)
    {
        m_spyMouse = enable;
    }

    bool Mouse::GetKey(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return mouseHold.test(pos);
    }

    bool Mouse::GetKeyDown(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return mouseDown.test(pos);
    }

    bool Mouse::GetKeyUp(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return mouseUp.test(pos);
    }

	void Mouse::SpyMouseEvent(Event e)
	{
		switch (e.type)
		{
		case EventType::MouseMove:
			Debug::Log(std::format(L"{}\t", e));
			Debug::LogLine(std::format(L"MouseX: {}\tMouseY: {}", m_position.x(), m_position.y()));
			break;

		case EventType::WheelRoll:
			Debug::Log(std::format(L"{}\t", e));
			Debug::LogLine(std::format(L"Raw wheel delta: {}\tWheel Delta: {}", RawWheelDelta(), WheelDelta()));
			break;

		default:
			Debug::LogLine(std::format(L"{}", e));
		}
	}

    bool Mouse::Contains(KeyCode k)
    {
        auto index = static_cast<unsigned char>(k);
        if (index > 0x01 && index <= 0x04)
            return true;
        return false;
    }
}