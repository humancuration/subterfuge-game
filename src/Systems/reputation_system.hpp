#ifndef REPUTATION_SYSTEM_HPP
#define REPUTATION_SYSTEM_HPP

#include <godot_cpp/classes/node.hpp>
#include <unordered_map>
#include <string>
#include "game_manager.hpp"

namespace game_systems {

class ReputationSystem : public godot::Node {
    GDCLASS(ReputationSystem, Node)

private:
    GameManager* game_manager;
    std::unordered_map<String, float> faction_reputation;
    std::unordered_map<String, float> npc_reputation;

protected:
    static void _bind_methods();

public:
    ReputationSystem();
    explicit ReputationSystem(GameManager* p_manager);

    void update(double delta);
    void modify_reputation(NPC* npc, float amount);
    float get_reputation(const NPC* npc) const;
    
    void set_faction_reputation(const String& faction, float value);
    float get_faction_reputation(const String& faction) const;
    
    void modify_global_reputation(const String& region, float amount);
};

}  // namespace game_systems

#endif // REPUTATION_SYSTEM_HPP 