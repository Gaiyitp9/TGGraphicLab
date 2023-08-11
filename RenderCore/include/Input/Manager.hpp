/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.hpp"
#include <tuple>

namespace TG::Input
{
    // 输入设备管理器
    template<typename... Devices> requires (sizeof...(Devices) > 0)
    class Manager
	{
        constexpr static std::size_t Count = sizeof...(Devices);

	public:
        Manager() = default;
        Manager(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager& operator=(Manager&&) = delete;
		~Manager() = default;

        // 更新各种输入设备的状态
		void Update() { Update<0>(); }
        // 广播输入事件
        void Broadcast(const Event& event) { Broadcast<0>(event); }
        // 监控输入事件
        void SpyEvent(bool enable) { SpyEvent<0>(enable); }
        // 监控指定设备的输入事件
        template<typename Device>
        void SpyEvent(bool enable)
        {
            constexpr std::size_t index = DeviceIndex<Device, 0>();
            if constexpr (index != static_cast<std::size_t>(-1))
                std::get<index>(m_devices).SpyEvent(enable);
        }
        // 是否按住按键
		[[nodiscard]] bool GetKey(KeyCode key) const { return GetKey<0>(key); }
        // 是否按下按键
		[[nodiscard]] bool GetKeyDown(KeyCode key) const { return GetKeyDown<0>(key); }
        // 是否释放按键
		[[nodiscard]] bool GetKeyUp(KeyCode key) const { return GetKeyUp<0>(key); }

    private:
        template<typename Device, std::size_t N>
        [[nodiscard]] consteval static std::size_t DeviceIndex()
        {
            if constexpr (N == Count)
                return static_cast<std::size_t>(-1);
            else if constexpr (std::is_same_v<Device, std::tuple_element_t<N, std::tuple<Devices...>>>)
                return N;
            else
                return DeviceIndex<Device, N + 1>();
        }

        // 编译期展开tuple
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
        void SpyEvent(bool enable)
        {
            if constexpr (N < Count)
            {
                std::get<N>(m_devices).SpyEvent(enable);
                SpyEvent<N + 1>(enable);
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
        std::tuple<Devices...> m_devices;       // 输入设备
	};
}