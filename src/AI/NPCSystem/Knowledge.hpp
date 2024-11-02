#pragma once
#include <string>
#include <vector>

struct Knowledge {
    enum class Type {
        ENVIRONMENTAL,
        CULTURAL,
        TECHNOLOGICAL,
        SOCIAL,
        SPIRITUAL
    };

    Type type;
    std::string content;
    float value{1.0f};
    bool can_be_shared{true};
    std::vector<std::string> prerequisites;
}; 