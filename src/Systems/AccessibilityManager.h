#ifndef ACCESSIBILITYMANAGER_H
#define ACCESSIBILITYMANAGER_H

#include <Godot.hpp>
#include <Node.hpp>

namespace Systems {

class AccessibilityManager : public godot::Node {
    GODOT_CLASS(AccessibilityManager, godot::Node)

public:
    enum ComplexityLevel {
        BASIC,
        INTERMEDIATE,
        ADVANCED
    };

private:
    ComplexityLevel current_complexity = INTERMEDIATE;

    bool auto_manage_research = false;
    bool auto_manage_city_development = false;
    bool simplified_economy = false;
    bool detailed_event_descriptions = true;

public:
    static void _register_methods();

    AccessibilityManager();
    ~AccessibilityManager();

    void _init(); // Called by Godot
    void set_complexity_level(ComplexityLevel level);
    ComplexityLevel get_current_complexity() const;

    void update_game_complexity();
    void simplify_ui();
    void automate_complex_systems();
    void balance_ui_complexity();
    void partially_automate_systems();
    void show_full_complex_ui();
    void disable_automation();

    godot::String get_tooltip(const godot::String &element_id);
    void start_tutorial();

    void toggle_auto_research(bool auto_manage);
    void toggle_auto_city_development(bool auto_manage);

    godot::String get_event_description(const godot::String &event_id);
    godot::String get_detailed_event_description(const godot::String &event_id);
    godot::String get_simplified_event_description(const godot::String &event_id);

    void adjust_ui_scale(float scale);
    void toggle_colorblind_mode(bool enable);
};

} // namespace Systems

#endif // ACCESSIBILITYMANAGER_H 