#ifndef ELECTION_MANAGER_HPP
#define ELECTION_MANAGER_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace Systems {

class Country;
class Candidate;

// Delegate type for election events
using ElectionEventHandler = std::function<void(const String& country_name, Candidate* winner)>;

class ElectionManager : public Node {
    GDCLASS(ElectionManager, Node)

private:
    std::vector<Country*> countries;
    double election_timer;
    double election_campaign_duration;
    bool campaign_phase_active;
    
    // Election event callbacks
    std::vector<ElectionEventHandler> election_start_callbacks;
    std::vector<ElectionEventHandler> election_end_callbacks;

    static void _bind_methods();
    void trigger_elections();
    void set_next_election_cycle();
    void start_campaign_phase();
    void end_campaign_phase();
    void update_campaign_effects(double delta);

protected:
    String serialize_election_state() const;
    void deserialize_election_state(const String& state);

public:
    ElectionManager();
    ~ElectionManager();

    void _ready() override;
    void _notification(int p_what);
    void update(double delta);
    
    // Configuration methods
    void set_election_cycle_duration(double duration);
    double get_election_cycle_duration() const;
    void set_campaign_duration(double duration);
    double get_campaign_duration() const;

    // Country management
    void initialize_with_countries(TypedArray<Country> p_countries);
    void add_country(Country* country);
    void remove_country(Country* country);
    
    // Event handling
    void add_election_start_callback(ElectionEventHandler callback);
    void add_election_end_callback(ElectionEventHandler callback);
    void clear_callbacks();

    // Campaign phase
    bool is_campaign_active() const { return campaign_phase_active; }
    float get_campaign_progress() const;
    
    // Debug/Editor tools
    void force_trigger_election(const String& country_name);
    Array get_active_candidates() const;
};

} // namespace Systems

#endif // ELECTION_MANAGER_HPP 