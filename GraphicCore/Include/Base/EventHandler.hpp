/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "TypeList.hpp"
#include <vector>

namespace TG
{
    // 类型列表中所有类型是否唯一
    template<typename T, typename... Types>
    constexpr bool AreTypesUnique = (!std::is_same_v<T, Types> && ...) && AreTypesUnique<Types...>;
    template<typename T>
    constexpr bool AreTypesUnique<T> = true;

    // 类型T是否在类型列表Types中
    template<typename T, typename... Types>
    constexpr bool IsInTypes = (std::is_same_v<T, Types> || ...);

    // 至少支持一种事件，且不存在重复事件
    template<typename... Events>
    concept UniqueEvents = sizeof...(Events) > 0 && AreTypesUnique<Events...>;

    // 事件处理器
    template<typename Event>
    struct IEventHandler
    {
        virtual ~IEventHandler() = default;
        virtual void Handle(const Event& event) = 0;
    };

    // 事件分发器
    template<UniqueEvents... Events>
    class EventDispatcher
    {
        // 事件处理器，用IEventHandler<Event>接口表示
        // 支持多个事件处理器，用vector记录事件处理器
        // 为了利用虚函数能力同时避免用指针(纯属个人品味)，用std::reference_wrapper保存事件处理器引用
        template<typename Event>
        using EventHandlers = std::vector<std::reference_wrapper<IEventHandler<Event>>>;

    public:
        template<typename Event> requires IsInTypes<Event, Events...>
        void Dispatch(const Event& event) const
        {
            for (const EventHandlers<Event>& eventHandlers =  std::get<EventHandlers<Event>>(m_allEventHandlers);
                const std::reference_wrapper<IEventHandler<Event>>& handler: eventHandlers)
                handler.get().Handle(event);
        }

        template<typename Event> requires IsInTypes<Event, Events...>
        void Register(IEventHandler<Event>& handler)
        {
            EventHandlers<Event>& eventHandlers =  std::get<EventHandlers<Event>>(m_allEventHandlers);
            eventHandlers.emplace_back(handler);
        }

        template<typename Event> requires IsInTypes<Event, Events...>
        void UnRegister(const IEventHandler<Event>& handler)
        {
            EventHandlers<Event>& eventHandlers =  std::get<EventHandlers<Event>>(m_allEventHandlers);
            std::erase_if(eventHandlers,
            [&handler](const std::reference_wrapper<IEventHandler<Event>>& refWrapper)
            {
                return std::addressof(refWrapper.get()) == std::addressof(handler);
            });
        }

    private:
        // 多个事件的处理器集合
        std::tuple<EventHandlers<Events>...> m_allEventHandlers;
    };

    // TypeList转换成EventDispatcher
    template<typename... Events>
    EventDispatcher<Events...> ConstructEventDispatcherFromTypeList(TypeList<Events...>) { return {}; }
    template<typename EventList>
    using EventDispatcherFromEventList = decltype(ConstructEventDispatcherFromTypeList(std::declval<EventList>()));
}
