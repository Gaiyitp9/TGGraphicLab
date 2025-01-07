/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostic/Log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <print>

namespace TG
{
    Log::Log()
    {
        // Spdlog will not throw exceptions while logging (since version 39cdd08).
        // It might throw during the construction of a logger or sink, because it is considered fatal.
        // https://github.com/gabime/spdlog/wiki/Error-handling
        try
        {
            m_logger = spdlog::basic_logger_mt("AsyncLogger", "DefaultLog.txt", true);
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            m_logger->sinks().emplace_back(consoleSink);
        }
        catch (const spdlog::spdlog_ex::exception& e)
        {
            std::println(stderr, "Spdlog initialization failed: {}", e.what());
        }
    }

    Log::~Log()
    {
        spdlog::drop_all();
    }
}
