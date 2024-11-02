#include "ModManager.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/directory_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace Data {

void ModManager::_bind_methods() {
    // Register signals
    ADD_SIGNAL(MethodInfo("mod_loaded", PropertyInfo(Variant::STRING, "mod_name")));
    ADD_SIGNAL(MethodInfo("mod_enabled", PropertyInfo(Variant::STRING, "mod_name")));
    ADD_SIGNAL(MethodInfo("mod_disabled", PropertyInfo(Variant::STRING, "mod_name")));
    
    // Register methods
    ClassDB::bind_method(D_METHOD("add_mod_search_path", "path"), &ModManager::add_mod_search_path);
    ClassDB::bind_method(D_METHOD("discover_mods"), &ModManager::discover_mods);
    ClassDB::bind_method(D_METHOD("enable_mod", "mod_id"), &ModManager::enable_mod);
    ClassDB::bind_method(D_METHOD("disable_mod", "mod_id"), &ModManager::disable_mod);
    ClassDB::bind_method(D_METHOD("get_mod_info", "mod_id"), &ModManager::get_mod_info);
    ClassDB::bind_method(D_METHOD("get_active_mods"), &ModManager::get_active_mods);
    ClassDB::bind_method(D_METHOD("get_available_mods"), &ModManager::get_available_mods);
}

ModManager::ModManager() {
    // Add default mod search paths
    String base_path = OS::get_singleton()->get_executable_path().get_base_dir();
    add_mod_search_path(base_path.path_join("mods"));
    add_mod_search_path("user://mods");
}

void ModManager::add_mod_search_path(const String& path) {
    if (!mod_search_paths.has(path)) {
        mod_search_paths.push_back(path);
    }
}

Array ModManager::discover_mods() {
    Array discovered_mods;
    
    for (const String& search_path : mod_search_paths) {
        Ref<DirectoryAccess> dir = DirectoryAccess::open(search_path);
        if (dir.is_valid()) {
            dir->list_dir_begin();
            String mod_dir = dir->get_next();
            
            while (!mod_dir.is_empty()) {
                if (dir->current_is_dir() && !mod_dir.begins_with(".")) {
                    String full_path = search_path.path_join(mod_dir);
                    String manifest_path = full_path.path_join("manifest.json");
                    
                    if (FileAccess::file_exists(manifest_path)) {
                        Dictionary mod_info = load_mod_manifest(manifest_path);
                        if (!mod_info.is_empty()) {
                            discovered_mods.push_back(mod_info);
                        }
                    }
                }
                mod_dir = dir->get_next();
            }
            
            dir->list_dir_end();
        }
    }
    
    return discovered_mods;
}

Dictionary ModManager::load_mod_manifest(const String& manifest_path) {
    Dictionary mod_info;
    
    Ref<FileAccess> f = FileAccess::open(manifest_path, FileAccess::READ);
    if (f.is_valid()) {
        String content = f->get_as_text();
        Variant parsed = JSON::parse_string(content);
        
        if (parsed.get_type() == Variant::DICTIONARY) {
            Dictionary manifest = parsed;
            
            // Validate required fields
            if (manifest.has("id") && manifest.has("name") && manifest.has("version")) {
                mod_info = manifest;
                String mod_id = manifest["id"];
                
                // Store metadata for later use
                if (!active_mods.has(mod_id)) {
                    active_mods[mod_id] = Dictionary();
                }
            }
        }
    }
    
    return mod_info;
}

Error ModManager::enable_mod(const String& mod_id) {
    if (!active_mods.has(mod_id)) {
        return ERR_DOES_NOT_EXIST;
    }
    
    Array dependencies = check_mod_dependencies(mod_id);
    if (!dependencies.is_empty()) {
        return ERR_CANT_RESOLVE;
    }
    
    Dictionary mod_info = get_mod_info(mod_id);
    if (mod_info.is_empty()) {
        return ERR_INVALID_DATA;
    }
    
    // Enable the mod
    Dictionary& mod_data = active_mods[mod_id];
    mod_data["enabled"] = true;
    
    emit_signal("mod_enabled", mod_id);
    UtilityFunctions::print("Enabled mod: ", mod_id);
    
    return OK;
}

void ModManager::disable_mod(const String& mod_id) {
    if (active_mods.has(mod_id)) {
        Dictionary& mod_data = active_mods[mod_id];
        mod_data["enabled"] = false;
        
        emit_signal("mod_disabled", mod_id);
        UtilityFunctions::print("Disabled mod: ", mod_id);
    }
}

Dictionary ModManager::get_mod_info(const String& mod_id) const {
    if (active_mods.has(mod_id)) {
        return active_mods[mod_id];
    }
    return Dictionary();
}

Array ModManager::get_active_mods() const {
    Array result;
    Array keys = active_mods.keys();
    
    for (int i = 0; i < keys.size(); i++) {
        String mod_id = keys[i];
        Dictionary mod_data = active_mods[mod_id];
        if (mod_data.get("enabled", false)) {
            result.push_back(mod_id);
        }
    }
    
    return result;
}

Array ModManager::get_available_mods() const {
    return active_mods.keys();
}

bool ModManager::is_mod_compatible(const String& mod_id) const {
    if (!active_mods.has(mod_id)) {
        return false;
    }
    
    Dictionary mod_info = active_mods[mod_id];
    String mod_version = mod_info.get("version", "");
    String required_game_version = mod_info.get("required_game_version", "");
    
    // TODO: Implement version compatibility checking
    return true;
}

Array ModManager::check_mod_dependencies(const String& mod_id) const {
    Array missing_dependencies;
    
    if (active_mods.has(mod_id)) {
        Dictionary mod_info = active_mods[mod_id];
        Array dependencies = mod_info.get("dependencies", Array());
        
        for (int i = 0; i < dependencies.size(); i++) {
            String dep_id = dependencies[i];
            if (!active_mods.has(dep_id) || !active_mods[dep_id].get("enabled", false)) {
                missing_dependencies.push_back(dep_id);
            }
        }
    }
    
    return missing_dependencies;
}

Array ModManager::get_mod_conflicts(const String& mod_id) const {
    Array conflicts;
    
    if (active_mods.has(mod_id)) {
        Dictionary mod_info = active_mods[mod_id];
        Array incompatible = mod_info.get("incompatible_with", Array());
        
        for (int i = 0; i < incompatible.size(); i++) {
            String conflict_id = incompatible[i];
            if (active_mods.has(conflict_id) && active_mods[conflict_id].get("enabled", false)) {
                conflicts.push_back(conflict_id);
            }
        }
    }
    
    return conflicts;
}

} 