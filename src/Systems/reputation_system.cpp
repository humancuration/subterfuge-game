#include "reputation_system.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace game_systems {

void ReputationSystem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta"), &ReputationSystem::update);
    ClassDB::bind_method(D_METHOD("modify_reputation", "npc", "amount"), &ReputationSystem::modify_reputation);
    ClassDB::bind_method(D_METHOD("get_reputation", "npc"), &ReputationSystem::get_reputation);
    ClassDB::bind_method(D_METHOD("set_faction_reputation", "faction", "value"), &ReputationSystem::set_faction_reputation);
    ClassDB::bind_method(D_METHOD("get_faction_reputation", "faction"), &ReputationSystem::get_faction_reputation);
    ClassDB::bind_method(D_METHOD("modify_global_reputation", "region", "amount"), &ReputationSystem::modify_global_reputation);
}

ReputationSystem::ReputationSystem() : game_manager(nullptr) {}

ReputationSystem::ReputationSystem(GameManager* p_manager) : game_manager(p_manager) {}

void ReputationSystem::update(double delta) {
    // Gradually normalize reputations over time
    const float NORMALIZATION_RATE = 0.1f;
    
    for (auto& pair : faction_reputation) {
        // Slowly move reputation towards neutral (50)
        float current = pair.second;
        float direction = (50.0f - current) > 0 ? 1.0f : -1.0f;
        pair.second += direction * NORMALIZATION_RATE * delta;
        pair.second = std::clamp(pair.second, 0.0f, 100.0f);
    }

    for (auto& pair : npc_reputation) {
        float current = pair.second;
        float direction = (50.0f - current) > 0 ? 1.0f : -1.0f;
        pair.second += direction * NORMALIZATION_RATE * delta * 0.5f; // NPCs normalize slower
        pair.second = std::clamp(pair.second, 0.0f, 100.0f);
    }
}

void ReputationSystem::modify_reputation(NPC* npc, float amount) {
    if (!npc) return;

    String npc_id = npc->get_unique_id();
    float current = npc_reputation[npc_id];
    npc_reputation[npc_id] = std::clamp(current + amount, 0.0f, 100.0f);

    // Also affect faction reputation if NPC belongs to one
    String faction = npc->get_faction();
    if (!faction.is_empty()) {
        float faction_current = faction_reputation[faction];
        faction_reputation[faction] = std::clamp(faction_current + amount * 0.5f, 0.0f, 100.0f);
    }
}

float ReputationSystem::get_reputation(const NPC* npc) const {
    if (!npc) return 50.0f; // Default neutral reputation
    
    String npc_id = npc->get_unique_id();
    auto it = npc_reputation.find(npc_id);
    return it != npc_reputation.end() ? it->second : 50.0f;
}

void ReputationSystem::set_faction_reputation(const String& faction, float value) {
    faction_reputation[faction] = std::clamp(value, 0.0f, 100.0f);
}

float ReputationSystem::get_faction_reputation(const String& faction) const {
    auto it = faction_reputation.find(faction);
    return it != faction_reputation.end() ? it->second : 50.0f;
}

void ReputationSystem::modify_global_reputation(const String& region, float amount) {
    // Affect all NPCs and factions in the region
    if (!game_manager) return;

    auto* region_ptr = game_manager->get_region(region);
    if (!region_ptr) return;

    // Affect all NPCs in the region
    for (const auto& npc : region_ptr->get_npcs()) {
        modify_reputation(npc, amount * 0.5f); // Reduced effect for mass changes
    }
}

} // namespace game_systems 