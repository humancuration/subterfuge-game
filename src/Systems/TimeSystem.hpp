#ifndef TIMESYSTEM_HPP
#define TIMESYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include "ISystem.hpp"
#include <vector>

namespace Systems {

class TimeSystem : public godot::Node, public ISystem {
    GODOT_CLASS(TimeSystem, godot::Node)

public:
    enum class DayPhase {
        DAWN,
        MORNING,
        NOON,
        AFTERNOON,
        DUSK,
        NIGHT,
        MIDNIGHT
    };

    struct GameDateTime {
        int year;
        int month;
        int day;
        int hour;
        float minute;
    };

private:
    float time_scale{1.0f};
    float elapsed_time{0.0f};
    GameDateTime current_date;
    DayPhase current_phase{DayPhase::DAWN};
    
    struct TimeListener {
        godot::Object* target;
        godot::String method;
    };
    
    std::vector<TimeListener> phase_change_listeners;
    std::vector<TimeListener> day_change_listeners;

public:
    static void _register_methods();
    
    TimeSystem();
    ~TimeSystem();

    void _init();
    void update(float delta_time) override;

    // Time control methods
    void set_time_scale(float scale);
    float get_time_scale() const { return time_scale; }
    void pause_time();
    void resume_time();

    // Time query methods
    DayPhase get_current_phase() const { return current_phase; }
    int get_current_year() const { return current_date.year; }
    int get_current_month() const { return current_date.month; }
    int get_current_day() const { return current_date.day; }
    int get_current_hour() const { return current_date.hour; }
    float get_current_minute() const { return current_date.minute; }
    
    // Event registration
    void register_phase_change_listener(godot::Object* target, const godot::String& method);
    void register_day_change_listener(godot::Object* target, const godot::String& method);

private:
    void update_time(float delta_time);
    void check_phase_change();
    void advance_date();
    void notify_phase_change();
    void notify_day_change();
    DayPhase calculate_day_phase(int hour, float minute) const;
    bool is_leap_year(int year) const;
    int get_days_in_month(int month, int year) const;
};

} // namespace Systems

#endif // TIMESYSTEM_HPP 