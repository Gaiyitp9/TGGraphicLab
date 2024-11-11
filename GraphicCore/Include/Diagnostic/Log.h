/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Singleton.hpp"
#include "Base/CommonDef.h"
#include "spdlog/spdlog.h"

namespace TG
{
    class TG_API Log final : public Singleton<Log>
    {
    public:
        Log();
        ~Log();

        template <typename... Args>
        void Info(std::format_string<Args...> fmt, Args &&...args)
        {
            m_logger->info(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Info(const T& msg)
        {
            m_logger->info(msg);
        }

        template <typename... Args>
        void Warn(std::format_string<Args...> fmt, Args &&...args)
        {
            m_logger->warn(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Warn(const T& msg)
        {
            m_logger->warn(msg);
        }

        template <typename... Args>
        void Error(std::format_string<Args...> fmt, Args &&...args)
        {
            m_logger->error(fmt, std::forward<Args>(args)...);
        }
        template <typename T>
        void Error(const T& msg)
        {
            m_logger->error(msg);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };
}
