#pragma once
#include <godot_cpp/classes/node.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../Core/JobSystem.hpp"
#include "../Core/ObjectPool.hpp"

class EventSystem : public godot::Node {
    GDCLASS(EventSystem, Node)

private:
    struct EventData {
        alignas(16) std::string id;
        alignas(16) std::string title_key;
        float probability{0.0f};
        std::vector<std::string> required_conditions;
        std::vector<std::function<void()>> effects;
    };

    struct EventInstance {
        const EventData* data;
        float lifetime{0.0f};
        bool is_active{false};
    };

    // Object pool for event instances to avoid allocations
    ObjectPool<EventInstance> event_pool;
    
    // SIMD-aligned probability data for batch processing
    alignas(32) std::vector<float> event_probabilities;
    alignas(32) std::vector<float> condition_weights;
    
    std::unordered_map<std::string, EventData> event_database;
    std::vector<EventInstance*> active_events;
    
    std::unique_ptr<JobSystem> job_system;
    static constexpr size_t BATCH_SIZE = 64;

protected:
    static void _bind_methods();

public:
    EventSystem();
    ~EventSystem();

    void register_event(const std::string& id, const EventData& data);
    void update_events_parallel(float delta);
    void trigger_event(const std::string& event_id);
    void batch_process_events();

private:
    void process_event_batch_simd(size_t start_idx, size_t count);
    bool check_conditions_simd(const std::vector<std::string>& conditions);
}; 