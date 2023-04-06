/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Manager.hpp"

namespace TG::Input
{
    Manager::Manager() = default;

    Manager::~Manager() = default;

    void Manager::SpyInputEvent(bool enable, DeviceType type)
    {
        if (type == DeviceType::None)
        {
            for (std::pair<const DeviceType, std::unique_ptr<Device>>& p : m_devices)
                p.second->SpyEvent(enable);
        }
        else if (m_devices.contains(type))
            m_devices[type]->SpyEvent(enable);
    }

    void Manager::AddDevice(DeviceType type)
    {
        if (!m_devices.contains(type))
            m_devices[type] = DeviceFactory::Create(type);
    }

    void Manager::RemoveDevice(DeviceType type)
    {
        m_devices.erase(type);
    }

    void Manager::Receive(const Event& event)
    {
        m_events.push(event);
    }

	void Manager::Update()
	{
        // 更新设备状态
        for (std::pair<const Input::DeviceType, std::unique_ptr<Device>>& d : m_devices)
            d.second->Update();

        // 分发输入事件
        while (!m_events.empty())
        {
            Event& e = m_events.front();
            if (m_devices.contains(e.device))
                m_devices[e.device]->Receive(e);
            m_events.pop();
        }
	}

	bool Manager::GetKey(KeyCode key) const
	{
        using type_device_pair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const type_device_pair& p){ return p.second->GetKey(key);});
	}

	bool Manager::GetKeyDown(KeyCode key) const
	{
        using type_device_pair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const type_device_pair& p){ return p.second->GetKeyDown(key);});
	}

	bool Manager::GetKeyUp(KeyCode key) const
	{
        using type_device_pair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const type_device_pair& p){ return p.second->GetKeyUp(key);});
	}
}