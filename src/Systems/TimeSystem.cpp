#include "TimeSystem.hpp"
#include <Math.hpp>

namespace Systems {

using namespace godot;

void TimeSystem::_register_methods() {
    register_method("_init", &TimeSystem::_init);
    register_method("update", &TimeSystem::update);
    register_method("set_time_scale", &TimeSystem::set_time_scale);
    register_method("pause_time", &TimeSystem::pause_time);
    register_method("resume_time", &TimeSystem::resume_time);
    
    register_method("get_current_phase", &TimeSystem::get_current_phase);
    register_method("get_current_year", &TimeSystem::get_current_year);
    register_method("get_current_month", &TimeSystem::get_current_month);
    register_method("get_current_day", &TimeSystem::get_current_day);
    register_method("get_current_hour", &TimeSystem::get_current_hour);
    register_method("get_current_minute", &TimeSystem::get_current_minute);
    
    register_method("register_phase_change_listener", &TimeSystem::register_phase_change_listener);
    register_method("register_day_change_listener", &TimeSystem::register_day_change_listener);
}

TimeSystem::TimeSystem() {
    current_date = {2024, 1, 1, 6, 0.0f}; // Start at dawn of Jan 1, 2024
}

TimeSystem::~TimeSystem() {}

void TimeSystem::_init() {}

void TimeSystem::update(float delta_time) {
    if (time_scale > 0.0f) {
        update_time(delta_time);
    }
}

void TimeSystem::set_time_scale(float scale) {
    time_scale = Math::clamp(scale, 0.0f, 100.0f);
}

void TimeSystem::pause_time() {
    time_scale = 0.0f;
}

void TimeSystem::resume_time() {
    time_scale = 1.0f;
}

void TimeSystem::update_time(float delta_time) {
    float scaled_delta = delta_time * time_scale;
    elapsed_time += scaled_delta;
    
    // Update minutes (1 real second = 1 game minute)
    current_date.minute += scaled_delta * 60.0f;
    
    // Handle minute overflow
    while (current_date.minute >= 60.0f) {
        current_date.minute -= 60.0f;
        current_date.hour++;
        
        if (current_date.hour >= 24) {
            current_date.hour = 0;
            advance_date();
        }
    }
    
    check_phase_change();
}

void TimeSystem::check_phase_change() {
    DayPhase new_phase = calculate_day_phase(current_date.hour, current_date.minute);
    if (new_phase != current_phase) {
        current_phase = new_phase;
        notify_phase_change();
    }
}

void TimeSystem::advance_date() {
    current_date.day++;
    
    int days_in_month = get_days_in_month(current_date.month, current_date.year);
    if (current_date.day > days_in_month) {
        current_date.day = 1;
        current_date.month++;
        
        if (current_date.month > 12) {
            current_date.month = 1;
            current_date.year++;
        }
    }
    
    notify_day_change();
}

DayPhase TimeSystem::calculate_day_phase(int hour, float minute) const {
    float total_hours = hour + minute / 60.0f;
    
    if (total_hours < 6.0f) return DayPhase::NIGHT;
    if (total_hours < 8.0f) return DayPhase::DAWN;
    if (total_hours < 11.0f) return DayPhase::MORNING;
    if (total_hours < 13.0f) return DayPhase::NOON;
    if (total_hours < 16.0f) return DayPhase::AFTERNOON;
    if (total_hours < 18.0f) return DayPhase::DUSK;
    if (total_hours < 22.0f) return DayPhase::NIGHT;
    return DayPhase::MIDNIGHT;
}

void TimeSystem::register_phase_change_listener(Object* target, const String& method) {
    phase_change_listeners.push_back({target, method});
}

void TimeSystem::register_day_change_listener(Object* target, const String& method) {
    day_change_listeners.push_back({target, method});
}

void TimeSystem::notify_phase_change() {
    for (const auto& listener : phase_change_listeners) {
        if (listener.target->has_method(listener.method)) {
            listener.target->call(listener.method, static_cast<int>(current_phase));
        }
    }
}

void TimeSystem::notify_day_change() {
    for (const auto& listener : day_change_listeners) {
        if (listener.target->has_method(listener.method)) {
            listener.target->call(listener.method, 
                Dictionary::make("year", current_date.year,
                               "month", current_date.month,
                               "day", current_date.day));
        }
    }
}

bool TimeSystem::is_leap_year(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int TimeSystem::get_days_in_month(int month, int year) const {
    static const int days_per_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    
    return days_per_month[month - 1];
}

} // namespace Systems 