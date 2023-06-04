/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Dispatcher.hpp"

namespace TG::Input
{
    Dispatcher::Dispatcher() = default;

    Dispatcher::~Dispatcher() = default;

    void Dispatcher::SpyInputEvent(bool enable, DeviceType type)
    {
        if (DeviceType::None == type)
        {
            for (std::pair<const DeviceType, std::unique_ptr<Device>>& p : m_devices)
                p.second->SpyEvent(enable);
        }
        else if (m_devices.contains(type))
            m_devices[type]->SpyEvent(enable);
    }

    void Dispatcher::AddDevice(DeviceType type)
    {
        if (!m_devices.contains(type))
            m_devices[type] = CreateDevice(type);
    }

    void Dispatcher::RemoveDevice(DeviceType type)
    {
        m_devices.erase(type);
    }

    void Dispatcher::Dispatch(const Event& event)
    {
        // 分发输入事件
        m_devices[event.device]->Receive(event);
    }

	void Dispatcher::Update()
	{
        // 更新设备状态
        for (std::pair<const Input::DeviceType, std::unique_ptr<Device>>& d : m_devices)
            d.second->Update();
	}

	bool Dispatcher::GetKey(KeyCode key) const
	{
        using TypeDevicePair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const TypeDevicePair& p){ return p.second->GetKey(key);});
	}

	bool Dispatcher::GetKeyDown(KeyCode key) const
	{
        using TypeDevicePair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const TypeDevicePair& p){ return p.second->GetKeyDown(key);});
	}

	bool Dispatcher::GetKeyUp(KeyCode key) const
	{
        using TypeDevicePair = std::pair<const Input::DeviceType, std::unique_ptr<Device>>;
        return std::ranges::any_of(m_devices, [&key](const TypeDevicePair& p){ return p.second->GetKeyUp(key);});
	}
}