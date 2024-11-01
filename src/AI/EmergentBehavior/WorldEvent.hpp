#pragma once
#include <string>
#include <variant>
#include <unordered_map>

class WorldEvent {
public:
    enum class EventType {
        WEATHER_CHANGE,
        ECONOMIC_CHANGE,
        SOCIAL_CHANGE,
        DISASTER
    };

    struct EventData {
        std::variant<int, float, std::string> value;
        float intensity{1.0f};
    };

private:
    EventType type;
    std::unordered_map<std::string, EventData> parameters;

public:
    WorldEvent(EventType eventType) : type(eventType) {}

    template<typename T>
    void set_parameter(const std::string& key, T value, float intensity = 1.0f) {
        parameters[key] = EventData{value, intensity};
    }

    const EventData* get_parameter(const std::string& key) const {
        auto it = parameters.find(key);
        return it != parameters.end() ? &it->second : nullptr;
    }

    EventType get_type() const { return type; }
}; 