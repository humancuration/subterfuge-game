#include "ConfigSettings.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

namespace Config {

void ConfigSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_background_color", "color"), &ConfigSettings::set_background_color);
    ClassDB::bind_method(D_METHOD("get_background_color"), &ConfigSettings::get_background_color);
    // Bind other properties...

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "background_color"), "set_background_color", "get_background_color");
    // Add other properties...
}

ConfigSettings::ConfigSettings() {
    // Set defaults from Constants
    background_color = Constants::BACKGROUND_COLOR;
    node_color = Constants::NODE_COLOR;
    edge_color = Constants::EDGE_COLOR;
    node_size = Constants::NODE_SIZE;
    edge_width = Constants::EDGE_WIDTH;
    data_file = Constants::DEFAULT_DATA_FILE;
    log_file = Constants::LOG_FILE;
    window_width = Constants::WINDOW_WIDTH;
    window_height = Constants::WINDOW_HEIGHT;
}

Ref<ConfigSettings> ConfigSettings::load_settings(const String& file_path) {
    Ref<ConfigSettings> settings;
    settings.instantiate();

    Ref<FileAccess> f = FileAccess::open(file_path, FileAccess::READ);
    if (f.is_valid()) {
        String json_str = f->get_as_text();
        Variant parsed = JSON::parse_string(json_str);
        
        if (parsed.get_type() == Variant::DICTIONARY) {
            Dictionary data = parsed;
            // Load values from dictionary
            settings->background_color = data.get("background_color", Constants::BACKGROUND_COLOR);
            // Load other properties...
        }
    }
    
    return settings;
}

void ConfigSettings::save_settings(const String& file_path) {
    Dictionary data;
    data["background_color"] = background_color;
    // Add other properties to dictionary...

    String json_str = JSON::stringify(data);
    
    Ref<FileAccess> f = FileAccess::open(file_path, FileAccess::WRITE);
    if (f.is_valid()) {
        f->store_string(json_str);
    }
}

} 