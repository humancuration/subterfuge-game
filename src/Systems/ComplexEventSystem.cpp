#include "ComplexEventSystem.h"

#include <Godot.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Json.hpp>
#include <GodotGlobal.hpp>
#include "EventManager.h"

namespace Systems {

using namespace godot;

void ComplexEventSystem::_register_methods() {
    register_method("_process", &ComplexEventSystem::update_system);
    register_signal<ComplexEventSystem>("event_triggered", "game_event", GODOT_VARIANT_TYPE_OBJECT);
}

ComplexEventSystem::ComplexEventSystem() {
    rnd = godot::RandomNumberGenerator::_new();
}

ComplexEventSystem::~ComplexEventSystem() {
    // Cleanup if needed
}

void ComplexEventSystem::_init() {
    // Initialization if needed
}

void ComplexEventSystem::load_events(const godot::String &file_path) {
    godot::File *file = godot::File::_new();
    Error err = file->open(file_path, godot::File::READ);
    if (err == GODOT_OK) {
        godot::String json = file->get_as_text();
        file->close();

        godot::JSON json_parser;
        godot::JSONParseResult result = json_parser.parse(json);
        if (result.error == godot::JSONParseResult::Error::OK) {
            godot::Dictionary event_data = result.result;
            godot::Array events = event_data["events"];

            for(int i = 0; i < events.size(); i++) {
                godot::Dictionary evt = events[i];
                ComplexGameEvent *game_event = ComplexGameEvent::_new();
                game_event->from_dictionary(evt);
                all_events.append(game_event);
            }
            Godot::print("Events loaded successfully.");
        } else {
            Godot::print_err("Error parsing event definitions: " + result.error_string);
        }
    } else {
        Godot::print_err("Event definitions file not found: " + file_path);
    }
}

void ComplexEventSystem::update_system(float delta_time) {
    if (should_trigger_event()) {
        ComplexGameEvent *new_event = select_random_eligible_event();
        if (new_event) {
            trigger_event(new_event);
        }
    }

    // Update active events if needed
}

bool ComplexEventSystem::should_trigger_event() {
    float chance = rnd->randf();
    return chance < 0.01f; // 1% chance each update
}

ComplexGameEvent* ComplexEventSystem::select_random_eligible_event() {
    godot::Array eligible_events;
    for(int i = 0; i < all_events.size(); i++) {
        ComplexGameEvent *game_event = Object::cast_to<ComplexGameEvent>(all_events[i]);
        if(game_event && game_event->conditions_is_met(game_manager)) {
            eligible_events.append(game_event);
        }
    }

    if(eligible_events.size() > 0) {
        int index = rnd->randi_range(0, eligible_events.size() - 1);
        return Object::cast_to<ComplexGameEvent>(eligible_events[index]);
    }

    return nullptr;
}

void ComplexEventSystem::trigger_event(ComplexGameEvent *game_event) {
    active_events.append(game_event);

    // Replace placeholders in the event description
    godot::String processed_description = process_event_description(game_event->get_description());

    // Get the localized title using the title_key
    godot::String localized_title = localization_manager->get_text(game_event->get_title_key());

    Godot::print("Event Triggered: " + localized_title + " - " + processed_description);

    // Emit signal to notify UI
    emit_signal("event_triggered", Object::cast_to<Object>(game_event));
}

void ComplexEventSystem::handle_player_choice(ComplexGameEvent *game_event, PlayerChoice *choice) {
    godot::Dictionary effects = choice->get_outcome()->get_effects();
    godot::Array keys = effects.keys();
    for(int i = 0; i < keys.size(); i++) {
        godot::String key = keys[i];
        float value = effects[key];
        apply_effect(key, value);
    }

    godot::Array next_event_ids = choice->get_outcome()->get_trigger_next_events();
    for(int i = 0; i < next_event_ids.size(); i++) {
        godot::String next_event_id = next_event_ids[i];
        ComplexGameEvent *next_event = nullptr;
        for(int j = 0; j < all_events.size(); j++) {
            ComplexGameEvent *evt = Object::cast_to<ComplexGameEvent>(all_events[j]);
            if(evt && evt->get_id() == next_event_id && evt->conditions_is_met(game_manager)) {
                next_event = evt;
                break;
            }
        }
        if(next_event) {
            trigger_event(next_event);
        }
    }

    active_events.erase(game_event);
    Godot::print("Player chose: " + choice->get_choice_text());
}

void ComplexEventSystem::apply_effect(const godot::String &stat_name, float value) {
    godot::String stat = stat_name.to_lower();
    if(stat == "morale") {
        for(int i = 0; i < game_manager->nodes.size(); i++) {
            NodeStats *stats = game_manager->nodes[i]->get_stats();
            stats->economic_prosperity = Math::clamp(stats->economic_prosperity + value, 0.0f, 100.0f);
        }
    }
    else if(stat == "resourceavailability") {
        for(int i = 0; i < game_manager->nodes.size(); i++) {
            NodeStats *stats = game_manager->nodes[i]->get_stats();
            stats->resource_availability = Math::clamp(stats->resource_availability + value, 0.0f, 100.0f);
        }
    }
    else if(stat == "economicprosperity") {
        game_manager->get_economy_system()->set_economic_prosperity(game_manager->get_economy_system()->get_economic_prosperity() + value);
    }
    else if(stat == "culturaldevelopment") {
        for(int i = 0; i < game_manager->nodes.size(); i++) {
            NodeStats *stats = game_manager->nodes[i]->get_stats();
            stats->cultural_development = Math::clamp(stats->cultural_development + value, 0.0f, 100.0f);
        }
    }
    else {
        Godot::print_err("Unknown effect stat: " + stat_name);
    }

    // Example: Trigger environmental effects based on certain stats
    if(stat == "economicprosperity" && value > 5.0f) {
        // Example: Start raining if economic prosperity increases significantly
        game_manager->get_environment_manager()->toggle_rain(true);
    }
}

void ComplexEventSystem::connect_event_notifier(godot::Node *target, const godot::String &method) {
    connect("event_triggered", target, method);
}

godot::String ComplexEventSystem::process_event_description(const godot::String &description) {
    godot::String processed = description;
    processed = processed.replace("{nodeName}", name_database->get_random_node_name());
    processed = processed.replace("{npcFirstName}", name_database->get_random_npc_first_name());
    processed = processed.replace("{npcLastName}", name_database->get_random_npc_last_name());
    processed = processed.replace("{cultureName}", name_database->get_random_culture_name());
    return processed;
}

} // namespace Systems 