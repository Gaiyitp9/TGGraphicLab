/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Singleton.hpp"
#include "spdlog/spdlog.h"

namespace TG
{
    class Log final : public Singleton<Log>
    {
    public:
        template <typename... Args>
        void Info(std::format_string<Args...> fmt, Args&&...args)
        {
            m_logger->info(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Info(const T& msg)
        {
            m_logger->info(msg);
        }

        template <typename... Args>
        void Warn(std::format_string<Args...> fmt, Args&&...args)
        {
            m_logger->warn(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Warn(const T& msg)
        {
            m_logger->warn(msg);
        }

        template <typename... Args>
        void Error(std::format_string<Args...> fmt, Args&&...args)
        {
            m_logger->error(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Error(const T& msg)
        {
            m_logger->error(msg);
        }

    private:
        friend class Singleton;
        Log();
        ~Log() override;

        std::shared_ptr<spdlog::logger> m_logger;
    };

    template<typename... Args>
    void LogInfo(std::format_string<Args...> fmt, Args&&... args)
    {
        Log::Instance().Info(fmt, std::forward<Args>(args)...);
    }

    template<typename T>
    void LogInfo(const T& msg)
    {
        Log::Instance().Info(msg);
    }

    template<typename... Args>
    void LogWarn(std::format_string<Args...> fmt, Args&&... args)
    {
        Log::Instance().Warn(fmt, std::forward<Args>(args)...);
    }

    template<typename T>
    void LogWarn(const T& msg)
    {
        Log::Instance().Warn(msg);
    }

    template<typename... Args>
    void LogError(std::format_string<Args...> fmt, Args&&... args)
    {
        Log::Instance().Error(fmt, std::forward<Args>(args)...);
    }

    template<typename T>
    void LogError(const T& msg)
    {
        Log::Instance().Error(msg);
    }
}
