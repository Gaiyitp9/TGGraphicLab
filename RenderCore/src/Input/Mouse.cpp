/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.hpp"
#include "Diagnostics/Log.hpp"
#include "Input/EventData.hpp"
#include <format>

namespace TG::Input
{
	Mouse::Mouse() = default;

	Mouse::~Mouse() = default;

	void Mouse::PreUpdate()
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
        if (m_spyMouse)
            SpyMouseEvent(e);
    }

    void Mouse::SpyEvent(bool enable)
    {
        m_spyMouse = enable;
    }

    bool Mouse::GetKey(KeyCode k)
    {
        auto pos = static_cast<std::size_t>(k);
        return mouseHold.test(pos);
    }

    bool Mouse::GetKeyDown(KeyCode k)
    {
        auto pos = static_cast<std::size_t>(k);
        return mouseDown.test(pos);
    }

    bool Mouse::GetKeyUp(KeyCode k)
    {
        auto pos = static_cast<std::size_t>(k);
        return mouseUp.test(pos);
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