#pragma once
#include <godot_cpp/classes/node.hpp>
#include "../Data/ModManager.hpp"
#include "Systems/ReputationSystem.hpp"
#include "Systems/WeatherSystem.hpp"

namespace Game {

class GameManager : public godot::Node {
    GDCLASS(GameManager, Node)

private:
    // Core systems
    Dictionary systems;
    Dictionary mod_hooks;
    
    // Singleton instance
    static GameManager* singleton;
    
    // Core game state
    Dictionary game_state;
    bool is_paused;

protected:
    static void _bind_methods();

public:
    GameManager();
    virtual ~GameManager();
    
    virtual void _ready() override;
    
    // Singleton access
    static GameManager* get_singleton() { return singleton; }
    
    // System management
    void register_system(const String& system_id, const Variant& system);
    Variant get_system(const String& system_id) const;
    
    // Mod support
    void register_mod_hook(const String& hook_id, const Callable& callback);
    void trigger_mod_hook(const String& hook_id, const Array& args);
    
    // Game state management
    void set_game_state(const String& key, const Variant& value);
    Variant get_game_state(const String& key) const;
    
    // Save/Load support with mod data
    Dictionary serialize_game_state() const;
    void deserialize_game_state(const Dictionary& state);
};

} 