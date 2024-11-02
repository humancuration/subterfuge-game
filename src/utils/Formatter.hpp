#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class Formatter {
public:
    static std::string format_number(float value, int decimals = 2) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(decimals) << value;
        return ss.str();
    }

    static std::string format_percentage(float value, int decimals = 1) {
        return format_number(value * 100, decimals) + "%";
    }

    static std::string format_time(float seconds) {
        int minutes = static_cast<int>(seconds) / 60;
        int remaining_seconds = static_cast<int>(seconds) % 60;
        
        std::stringstream ss;
        ss << minutes << ":" << std::setfill('0') << std::setw(2) << remaining_seconds;
        return ss.str();
    }

    static std::string format_large_number(int64_t number) {
        if (number >= 1000000000) {
            return format_number(number / 1000000000.0f) + "B";
        }
        if (number >= 1000000) {
            return format_number(number / 1000000.0f) + "M";
        }
        if (number >= 1000) {
            return format_number(number / 1000.0f) + "K";
        }
        return std::to_string(number);
    }
}; 