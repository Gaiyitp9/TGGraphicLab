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
        if (DeviceType::None == type)
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
            m_devices[type] = CreateDevice(type);
    }

    void Manager::RemoveDevice(DeviceType type)
    {
        m_devices.erase(type);
    }

    void Manager::Dispatch(const Event& event)
    {
        // 分发输入事件
        DeviceType device = GetDeviceType(event.key);
        m_devices[device]->Receive(event);
    }

	void Manager::Update()
	{
        // 更新设备状态
        for (std::pair<const Input::DeviceType, std::unique_ptr<Device>>& d : m_devices)
            d.second->Update();
	}

	bool Manager::GetKey(KeyCode key) const
	{
        DeviceType device = GetDeviceType(key);
        return m_devices.at(device)->GetKey(key);
	}

	bool Manager::GetKeyDown(KeyCode key) const
	{
        DeviceType device = GetDeviceType(key);
        return m_devices.at(device)->GetKeyDown(key);
	}

	bool Manager::GetKeyUp(KeyCode key) const
	{
        DeviceType device = GetDeviceType(key);
        return m_devices.at(device)->GetKeyUp(key);
	}

    DeviceType Manager::GetDeviceType(KeyCode key)
    {
        auto index = static_cast<unsigned char>(key);
        if (index >= 0 && index <= 0x04)
            return DeviceType::Mouse;
        if (index >= 0x08 && index <= 0xDE)
            return DeviceType::Keyboard;
        return DeviceType::None;
    }
}