#ifndef COMPLEXEVENTSYSTEM_H
#define COMPLEXEVENTSYSTEM_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Vector.hpp>
#include <List.hpp>
#include "ComplexGameEvent.h"
#include "GameManager.h"
#include "NameDatabase.h"
#include "LocalizationManager.h"

namespace Systems {

class ComplexEventSystem : public godot::Node {
    GODOT_CLASS(ComplexEventSystem, godot::Node)

private:
    godot::Array all_events; // Array of ComplexGameEvent
    godot::Array active_events; // Array of ComplexGameEvent
    GameManager *game_manager;
    godot::RandomNumberGenerator *rnd;
    NameDatabase *name_database;
    LocalizationManager *localization_manager;

public:
    static void _register_methods();

    ComplexEventSystem();
    ~ComplexEventSystem();

    void _init(); // Called by Godot

    void load_events(const godot::String &file_path);
    void update_system(float delta_time);
    bool should_trigger_event();
    ComplexGameEvent* select_random_eligible_event();
    void trigger_event(ComplexGameEvent *game_event);
    void handle_player_choice(ComplexGameEvent *game_event, PlayerChoice *choice);
    void apply_effect(const godot::String &stat_name, float value);
    void connect_event_notifier(godot::Node *target, const godot::String &method = "DisplayEvent");
    godot::String process_event_description(const godot::String &description);
};

} // namespace Systems

#endif // COMPLEXEVENTSYSTEM_H 