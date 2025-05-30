/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    // Meyer单例基类
    template<typename T>
    class Singleton
    {
    public:
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton &operator=(const Singleton&) = delete;
        Singleton &operator=(Singleton&&) = delete;

        static T& Instance()
        {
            static T instance;
            return instance;
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    };
}
