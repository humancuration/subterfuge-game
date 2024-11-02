#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <string>
#include <memory>
#include "ConfigConstants.hpp"

namespace Config {
    class ConfigSettings : public godot::Resource {
        GDCLASS(ConfigSettings, Resource)

    private:
        // Visualization Parameters
        String background_color;
        String node_color;
        String edge_color;
        int node_size;
        int edge_width;

        // File Paths
        String data_file;
        String log_file;

        // UI Constants
        int window_width;
        int window_height;

    protected:
        static void _bind_methods();

    public:
        ConfigSettings();
        
        // Getters and setters
        void set_background_color(const String& p_color) { background_color = p_color; }
        String get_background_color() const { return background_color; }
        
        // Add similar getters/setters for other properties...

        static Ref<ConfigSettings> load_settings(const String& file_path);
        void save_settings(const String& file_path);
    };
} 