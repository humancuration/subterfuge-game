#ifndef EVENT_CONDITION_HPP
#define EVENT_CONDITION_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <string>
#include <map>
#include <functional>

namespace Systems {

class GameManager;

// Custom condition evaluator type
using ConditionEvaluator = std::function<bool(GameManager*, const String&)>;

class EventCondition : public RefCounted {
    GDCLASS(EventCondition, RefCounted)

private:
    String time_phase;
    String weather;
    String population_density;
    Dictionary custom_conditions;
    Dictionary cached_results;
    double cache_timeout;
    double last_cache_time;

    static std::map<String, ConditionEvaluator> custom_evaluators;
    
    static void _bind_methods();
    bool check_time_phase(GameManager* game_manager) const;
    bool check_weather(GameManager* game_manager) const;
    bool check_population_density(GameManager* game_manager) const;
    bool check_custom_conditions(GameManager* game_manager) const;
    bool check_custom_condition(const String& key, const String& value, GameManager* game_manager) const;
    double get_stat_value(const String& stat_name, GameManager* game_manager) const;
    double parse_population_density(const String& density) const;
    void clear_cache();

protected:
    bool evaluate_complex_condition(const String& condition, GameManager* game_manager) const;
    void cache_result(const String& key, bool result);
    bool get_cached_result(const String& key, bool& out_result) const;

public:
    EventCondition();
    ~EventCondition();

    bool is_met(GameManager* game_manager) const;
    float get_condition_weight(const String& condition_name) const;
    Dictionary get_condition_status(GameManager* game_manager) const;

    // Custom condition registration
    static void register_custom_evaluator(const String& type, ConditionEvaluator evaluator);
    static void unregister_custom_evaluator(const String& type);

    // Getters and setters
    void set_time_phase(const String& p_time_phase);
    String get_time_phase() const;
    void set_weather(const String& p_weather);
    String get_weather() const;
    void set_population_density(const String& p_density);
    String get_population_density() const;
    void set_custom_conditions(const Dictionary& p_conditions);
    Dictionary get_custom_conditions() const;
    void set_cache_timeout(double timeout);
    double get_cache_timeout() const;

    // Utility methods
    bool has_condition(const String& condition_name) const;
    Array get_active_conditions() const;
    void merge_conditions(const Ref<EventCondition>& other);
};

} // namespace Systems

#endif // EVENT_CONDITION_HPP 