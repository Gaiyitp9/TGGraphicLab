/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "EventHandler.h"
#include <tuple>

namespace TG::Input
{
    // 输入设备概念
    template<typename Device>
    concept InputDevice = requires(Device device)
    {
        device.Update();
        device.Receive(Event{});
        device.GetKey(KeyCode::None);
        device.GetKeyDown(KeyCode::None);
        device.GetKeyUp(KeyCode::None);
    };

    // 输入设备管理器
    template<InputDevice... Devices>
    class Manager final : public IEventHandler
	{
        constexpr static std::size_t Count = sizeof...(Devices);

	public:
        Manager() = default;
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(Manager&&) = delete;
		~Manager() override = default;

        // 更新各种输入设备的状态，每帧处理窗口输入事件前调用
		void Update() { Update<0>(); }
        void Consume(const Event& event) override { Broadcast<0>(event); }
		[[nodiscard]] bool GetKey(KeyCode key) const { return GetKey<0>(key); }
		[[nodiscard]] bool GetKeyDown(KeyCode key) const { return GetKeyDown<0>(key); }
		[[nodiscard]] bool GetKeyUp(KeyCode key) const { return GetKeyUp<0>(key); }

    private:
        template<typename Device, std::size_t N>
        consteval static std::size_t DeviceIndex()
        {
            if constexpr (N >= Count)
                return static_cast<std::size_t>(-1);
            else if constexpr (std::is_same_v<Device, std::tuple_element_t<N, std::tuple<Devices...>>>)
                return N;
            else
                return DeviceIndex<Device, N + 1>();
        }

        template<std::size_t N>
        void Update()
        {
            if constexpr (N < Count)
            {
                std::get<N>(m_devices).Update();
                Update<N + 1>();
            }
        }

        template<std::size_t N>
        void Broadcast(const Event& event)
        {
            if constexpr (N < Count)
            {
                std::get<N>(m_devices).Receive(event);
                Broadcast<N + 1>(event);
            }
        }

        template<std::size_t N>
        [[nodiscard]] bool GetKey(KeyCode key) const
        {
            if constexpr (N < Count)
                return std::get<N>(m_devices).GetKey(key) || GetKey<N + 1>(key);
            return false;
        }

        template<std::size_t N>
        [[nodiscard]] bool GetKeyDown(KeyCode key) const
        {
            if constexpr (N < Count)
                return std::get<N>(m_devices).GetKeyDown(key) || GetKeyDown<N + 1>(key);
            return false;
        }

        template<std::size_t N>
        [[nodiscard]] bool GetKeyUp(KeyCode key) const
        {
            if constexpr (N < Count)
                return std::get<N>(m_devices).GetKeyUp(key) || GetKeyUp<N + 1>(key);
            return false;
        }

	private:
        std::tuple<Devices...> m_devices;
	};
}
