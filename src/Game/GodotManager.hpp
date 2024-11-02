#pragma once
#include <godot_cpp/classes/node.hpp>
#include "GameManager.hpp"
#include "../UI/UIManager.hpp"

namespace Game {

class GodotManager : public godot::Node {
    GDCLASS(GodotManager, Node)

private:
    static GodotManager* singleton;
    
    Ref<GameManager> game_manager;
    Dictionary registered_ui_elements;
    Dictionary mod_ui_hooks;

protected:
    static void _bind_methods();

public:
    GodotManager();
    virtual ~GodotManager();
    
    virtual void _ready() override;
    
    // Singleton access
    static GodotManager* get_singleton() { return singleton; }
    
    // UI Registration for mods
    void register_ui_element(const String& element_id, const Variant& element);
    void register_ui_hook(const String& hook_id, const Callable& callback);
    
    // Mod UI integration
    void add_mod_menu_item(const String& mod_id, const String& menu_path, const Callable& callback);
    void add_mod_overlay(const String& mod_id, const Variant& overlay_node);
    
    // Event handling
    void emit_game_event(const String& event_type, const Dictionary& event_data);
};

} 