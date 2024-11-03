#include "event_outcome.hpp"

void EventOutcome::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_effects", "effects"), &EventOutcome::set_effects);
    ClassDB::bind_method(D_METHOD("get_effects"), &EventOutcome::get_effects);
    ClassDB::bind_method(D_METHOD("set_trigger_next_events", "events"), &EventOutcome::set_trigger_next_events);
    ClassDB::bind_method(D_METHOD("get_trigger_next_events"), &EventOutcome::get_trigger_next_events);

    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "effects"), "set_effects", "get_effects");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "trigger_next_events"), "set_trigger_next_events", "get_trigger_next_events");
}

EventOutcome::EventOutcome() {
    effects = Dictionary();
    trigger_next_events = Array();
}

EventOutcome::~EventOutcome() {}

void EventOutcome::set_effects(const Dictionary& p_effects) {
    effects = p_effects;
}

Dictionary EventOutcome::get_effects() const {
    return effects;
}

void EventOutcome::set_trigger_next_events(const Array& p_events) {
    trigger_next_events = p_events;
}

Array EventOutcome::get_trigger_next_events() const {
    return trigger_next_events;
} 