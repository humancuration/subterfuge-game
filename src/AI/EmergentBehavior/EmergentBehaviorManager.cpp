#include "EmergentBehaviorManager.hpp"
#include <algorithm>

EmergentBehaviorManager* EmergentBehaviorManager::instance = nullptr;

EmergentBehaviorManager& EmergentBehaviorManager::get_instance() {
    if (!instance) {
        instance = new EmergentBehaviorManager();
    }
    return *instance;
}

void EmergentBehaviorManager::register_npc(const std::shared_ptr<NPCController>& npc) {
    npcs.push_back(npc);
}

void EmergentBehaviorManager::process_event(const WorldEvent& event) {
    // Clean up expired NPCs
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(),
            [](const std::weak_ptr<NPCController>& weak_npc) {
                return weak_npc.expired();
            }
        ),
        npcs.end()
    );

    // Process NPCs in parallel batches
    std::vector<std::future<void>> futures;
    for (size_t i = 0; i < npcs.size(); i += BATCH_SIZE) {
        size_t batch_end = std::min(i + BATCH_SIZE, npcs.size());
        std::vector<std::weak_ptr<NPCController>> batch(
            npcs.begin() + i, 
            npcs.begin() + batch_end
        );
        
        futures.push_back(
            threadPool->enqueue([this, batch, event]() {
                process_npc_batch(batch, event);
            })
        );
    }

    // Wait for all batches to complete
    for (auto& future : futures) {
        future.wait();
    }
}

void EmergentBehaviorManager::process_npc_batch(
    const std::vector<std::weak_ptr<NPCController>>& batch,
    const WorldEvent& event) {
    
    for (const auto& weak_npc : batch) {
        if (auto npc = weak_npc.lock()) {
            switch (event.get_type()) {
                case WorldEvent::EventType::WEATHER_CHANGE:
                    if (auto weather_data = event.get_parameter("weather")) {
                        if (const auto* weather_value = 
                            std::get_if<std::string>(&weather_data->value)) {
                            npc->react_to_weather(WeatherCondition::Storm);
                        }
                    }
                    break;
                // Handle other event types...
            }
        }
    }
}

void EmergentBehaviorManager::update(float delta_time) {
    for (const auto& weak_npc : npcs) {
        if (auto npc = weak_npc.lock()) {
            npc->update(delta_time);
        }
    }
} 