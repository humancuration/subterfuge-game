#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace Data {

class ModManager : public godot::Node {
    GDCLASS(ModManager, Node)

private:
    Dictionary active_mods;
    Dictionary mod_load_order;
    Vector<String> mod_search_paths;

protected:
    static void _bind_methods();

public:
    ModManager();
    
    void add_mod_search_path(const String& path);
    Array discover_mods();
    Error enable_mod(const String& mod_id);
    void disable_mod(const String& mod_id);
    
    Dictionary get_mod_info(const String& mod_id) const;
    Array get_active_mods() const;
    Array get_available_mods() const;
    
    // Mod validation and compatibility
    bool is_mod_compatible(const String& mod_id) const;
    Array check_mod_dependencies(const String& mod_id) const;
    Array get_mod_conflicts(const String& mod_id) const;
};

} 