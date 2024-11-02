#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logger {
private:
    static std::shared_ptr<spdlog::logger> logger;

public:
    static void setup(const std::string& name, 
                     const std::string& log_file_path) {
        try {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                log_file_path, true);

            std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
            logger = std::make_shared<spdlog::logger>(
                name, sinks.begin(), sinks.end());

            logger->set_level(spdlog::level::info);
            logger->flush_on(spdlog::level::info);
            
            spdlog::register_logger(logger);
        }
        catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        }
    }

    static void log_info(const std::string& message) {
        if (logger)
            logger->info(message);
    }

    static void log_error(const std::string& message) {
        if (logger)
            logger->error(message);
    }

    static void log_debug(const std::string& message) {
        if (logger)
            logger->debug(message);
    }
};

std::shared_ptr<spdlog::logger> Logger::logger = nullptr; 