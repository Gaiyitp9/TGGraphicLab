/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.h"
#include "Diagnostics/Log.h"
#include <format>

namespace TG::Input
{
	void Mouse::Update()
	{
		m_mouseDown.reset();
		m_mouseUp.reset();
        m_wheelDelta = 0;
	}

    void Mouse::Receive(const Event& e)
    {
        // 碰到非鼠标按键直接返回
        if (!IsMouseKey(e.key)) return;

        auto key = static_cast<std::size_t>(e.key);
        switch (e.type)
        {
            case EventType::Press:
                m_mouseDown[key] = true;
                break;
            case EventType::Release:
                m_mouseUp[key] = true;
                break;
            case EventType::MouseMove:
            {
                auto data = std::any_cast<MouseData>(&e.data);
                assert(data && "Mouse move event doesn't contain data or the data type is not MouseData");
                m_position.x() = data->x;
                m_position.y() = data->y;
                break;
            }

            case EventType::WheelRoll:
            {
                auto data = std::any_cast<MouseData>(&e.data);
                assert(data && "WheelRoll event doesn't contain data or the data type is not MouseData");
                m_wheelDelta = data->delta;
                break;
            }
            default:
                break;
        }

        // 监控鼠标
        if (m_spyEvent)
            SpyMouseEvent(e);
    }

    void Mouse::SpyEvent(bool enable)
    {
        m_spyEvent = enable;
    }

    bool Mouse::GetKey(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        auto pos = static_cast<std::size_t>(k);
        return m_mouseHold.test(pos);
    }

    bool Mouse::GetKeyDown(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        auto pos = static_cast<std::size_t>(k);
        return m_mouseDown.test(pos);
    }

    bool Mouse::GetKeyUp(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        auto pos = static_cast<std::size_t>(k);
        return m_mouseUp.test(pos);
    }

	void Mouse::SpyMouseEvent(const Event& e)
	{
        Debug::Log(std::format("Key: {:<20} Event: {:<20} ", EventInfo::keysName[e.key], EventInfo::eventTypes[e.type]));
		switch (e.type)
		{
		case EventType::MouseMove:
			Debug::LogLine(std::format("MouseX: {:<20} MouseY: {:<20}", m_position.x(), m_position.y()));
			break;

		case EventType::WheelRoll:
			Debug::LogLine(std::format("Raw wheel delta: {:<20} Wheel Delta: {:<20}", RawWheelDelta(), WheelDelta()));
			break;

		default:
            Debug::LogLine("");
            break;
		}
	}
}