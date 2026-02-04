/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Diagnostic/Log.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
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
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::info);
            console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%n] [%^%l%$] %v");

            // Create a file rotating logger with 5mb size max and 3 rotated files.
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Logs/DefaultLog.txt",
                1024 * 1024 * 5, 3);
            file_sink->set_level(spdlog::level::trace);
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%n] [%^%l%$] %v");

            m_logger = std::make_shared<spdlog::logger>("TGLogger",
                spdlog::sinks_init_list{ console_sink, file_sink });
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
