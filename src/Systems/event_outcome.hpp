#ifndef EVENT_OUTCOME_HPP
#define EVENT_OUTCOME_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <functional>
#include <map>

namespace Systems {

class GameManager;

// Delegate type for custom effect handlers
using EffectHandler = std::function<void(GameManager*, const String&, float)>;

class EventOutcome : public RefCounted {
    GDCLASS(EventOutcome, RefCounted)

private:
    Dictionary effects;
    Array trigger_next_events;
    Dictionary delayed_effects;
    Dictionary conditional_effects;
    float effect_multiplier;
    
    static std::map<String, EffectHandler> custom_effect_handlers;
    
    static void _bind_methods();
    void process_delayed_effects(GameManager* game_manager);
    bool evaluate_condition(const String& condition, GameManager* game_manager) const;

protected:
    void apply_effect(const String& effect_type, float value, GameManager* game_manager);
    void schedule_delayed_effect(const String& effect_type, float value, float delay);

public:
    EventOutcome();
    ~EventOutcome();

    // Core functionality
    void apply_outcomes(GameManager* game_manager);
    void update(float delta_time, GameManager* game_manager);
    
    // Effect management
    void add_effect(const String& effect_type, float value);
    void add_delayed_effect(const String& effect_type, float value, float delay);
    void add_conditional_effect(const String& effect_type, float value, const String& condition);
    void remove_effect(const String& effect_type);
    
    // Event chain management
    void add_next_event(const String& event_id);
    void remove_next_event(const String& event_id);
    bool has_next_event(const String& event_id) const;
    
    // Custom effect handlers
    static void register_effect_handler(const String& type, EffectHandler handler);
    static void unregister_effect_handler(const String& type);
    
    // Getters and setters
    void set_effects(const Dictionary& p_effects);
    Dictionary get_effects() const;
    void set_trigger_next_events(const Array& p_events);
    Array get_trigger_next_events() const;
    void set_effect_multiplier(float multiplier);
    float get_effect_multiplier() const;
    
    // Utility methods
    Dictionary get_active_effects() const;
    Array get_pending_delayed_effects() const;
    void clear_all_effects();
    void merge_outcomes(const Ref<EventOutcome>& other);
};

} // namespace Systems

#endif // EVENT_OUTCOME_HPP 