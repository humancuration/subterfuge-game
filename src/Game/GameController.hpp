#pragma once
#include <godot_cpp/classes/node.hpp>
#include "../Data/ModManager.hpp"
#include "../Data/DataProcessor.hpp"

namespace Game {

class GameController : public godot::Node {
    GDCLASS(GameController, Node)

private:
    Ref<Data::ModManager> mod_manager;
    Ref<Data::DataProcessor> data_processor;
    
    // Core systems that can be extended by mods
    Dictionary systems;
    Dictionary registered_actions;
    Dictionary registered_buildings;

protected:
    static void _bind_methods();

public:
    GameController();
    virtual ~GameController();

    virtual void _ready() override;
    
    // System Registration (for mods)
    void register_game_system(const String& system_id, const Variant& system);
    void register_action(const String& action_id, const Callable& callback);
    void register_building_type(const Dictionary& building_data);
    
    // Core gameplay methods that mods can hook into
    Error process_node_visit(const Variant& node);
    Error build_on_node(const Variant& node, const Dictionary& building_data);
    Error upgrade_building(const Variant& node, const Variant& building);
    
    // Mod hooks
    Array get_available_actions(const Variant& context) const;
    Dictionary get_building_types() const;
    
    // Turn processing
    void process_turn();
    
    // Signals
    void emit_game_event(const String& event_type, const Dictionary& event_data);
};

} 