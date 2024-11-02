#pragma once
#include <godot_cpp/classes/node.hpp>
#include "DataTypes.hpp"

namespace Data {

enum class DataFormat {
    JSON,
    YAML,
    TOML,
    XML
};

class DataProcessor : public godot::Node {
    GDCLASS(DataProcessor, Node)

private:
    Dictionary loaded_data;
    Dictionary mod_data;
    Vector<String> mod_paths;
    
    // Helper methods for different formats
    Variant parse_data(const String& content, DataFormat format);
    String serialize_data(const Variant& data, DataFormat format);

public:
    DataProcessor();
    
    // Core data loading
    Error load_base_data(const String& base_path);
    Error load_mod(const String& mod_path);
    void unload_mod(const String& mod_id);
    
    // Data access methods
    Variant get_data(const String& category, const String& key) const;
    Array get_all_events() const;
    Array get_all_candidates() const;
    Dictionary get_localized_text(const String& language_code) const;
    
    // Plugin/Mod support
    void register_mod_path(const String& path);
    void scan_for_mods();
    Array get_available_mods() const;
    
    // Data processing utilities
    Dictionary process_genetic_data(const Array& raw_data);
    Dictionary calculate_distances(const Array& markers);
    
    // New format-specific methods
    Error load_mod_data(const String& path, DataFormat format = DataFormat::YAML);
    Error save_mod_data(const String& path, const Dictionary& data, DataFormat format = DataFormat::YAML);
    
    // Format detection
    DataFormat detect_format(const String& file_path) const;
};

} 