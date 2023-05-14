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
                if (e.data)
                {
                    auto const data = static_cast<MouseData*>(e.data);
                    m_position.x() = data->x;
                    m_position.y() = data->y;
                }
                break;
            }

            case EventType::WheelRoll:
            {
                if (e.data)
                    m_wheelDelta = static_cast<MouseData*>(e.data)->delta;
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
        Debug::Log(std::format(L"Key: {:<20} Event: {:<20} ", EventInfo::keysName[e.key], EventInfo::eventTypes[e.type]));
		switch (e.type)
		{
		case EventType::MouseMove:
			Debug::LogLine(std::format(L"MouseX: {:<20} MouseY: {:<20}", m_position.x(), m_position.y()));
			break;

		case EventType::WheelRoll:
			Debug::LogLine(std::format(L"Raw wheel delta: {:<20} Wheel Delta: {:<20}", RawWheelDelta(), WheelDelta()));
			break;

		default:
            Debug::LogLine(L"");
            break;
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