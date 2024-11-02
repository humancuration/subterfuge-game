#include "DataProcessor.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "yaml-cpp/yaml.h"
#include "toml++/toml.h"
#include "tinyxml2.h"

namespace Data {

void DataProcessor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_base_data", "base_path"), &DataProcessor::load_base_data);
    ClassDB::bind_method(D_METHOD("load_mod", "mod_path"), &DataProcessor::load_mod);
    ClassDB::bind_method(D_METHOD("unload_mod", "mod_id"), &DataProcessor::unload_mod);
    ClassDB::bind_method(D_METHOD("get_data", "category", "key"), &DataProcessor::get_data);
    ClassDB::bind_method(D_METHOD("get_all_events"), &DataProcessor::get_all_events);
    ClassDB::bind_method(D_METHOD("get_all_candidates"), &DataProcessor::get_all_candidates);
    ClassDB::bind_method(D_METHOD("scan_for_mods"), &DataProcessor::scan_for_mods);
    ClassDB::bind_method(D_METHOD("get_available_mods"), &DataProcessor::get_available_mods);
}

DataProcessor::DataProcessor() {}

Error DataProcessor::load_base_data(const String& base_path) {
    Error err = OK;
    
    // Load core data files
    Vector<String> core_files = {
        "EventDefinitions.json",
        "CandidateDefinitions.json",
        "ElectionDefinitions.json",
        "Names.json",
        "LocalizedText_en.json"
    };
    
    for (const String& file : core_files) {
        String full_path = base_path.path_join(file);
        Ref<FileAccess> f = FileAccess::open(full_path, FileAccess::READ);
        
        if (f.is_valid()) {
            String content = f->get_as_text();
            Variant parsed = JSON::parse_string(content);
            
            if (parsed.get_type() == Variant::DICTIONARY) {
                String category = file.get_basename().to_lower();
                loaded_data[category] = parsed;
            } else {
                UtilityFunctions::print("Failed to parse ", full_path);
                err = FAILED;
            }
        }
    }
    
    return err;
}

Error DataProcessor::load_mod(const String& mod_path) {
    // Load mod manifest
    String manifest_path = mod_path.path_join("manifest.json");
    Ref<FileAccess> f = FileAccess::open(manifest_path, FileAccess::READ);
    
    if (!f.is_valid()) {
        return ERR_FILE_NOT_FOUND;
    }
    
    String content = f->get_as_text();
    Dictionary manifest = JSON::parse_string(content);
    
    String mod_id = manifest.get("id", "");
    if (mod_id.is_empty()) {
        return ERR_INVALID_DATA;
    }
    
    // Load mod data files
    Array data_files = manifest.get("data_files", Array());
    Dictionary mod_content;
    
    for (int i = 0; i < data_files.size(); i++) {
        String file_path = mod_path.path_join(data_files[i]);
        Ref<FileAccess> data_file = FileAccess::open(file_path, FileAccess::READ);
        
        if (data_file.is_valid()) {
            String file_content = data_file->get_as_text();
            Variant parsed = JSON::parse_string(file_content);
            
            if (parsed.get_type() == Variant::DICTIONARY) {
                String category = file_path.get_basename().to_lower();
                mod_content[category] = parsed;
            }
        }
    }
    
    mod_data[mod_id] = mod_content;
    return OK;
}

Dictionary DataProcessor::process_genetic_data(const Array& raw_data) {
    Dictionary result;
    Dictionary species_markers;
    
    // Process raw genetic data
    for (int i = 0; i < raw_data.size(); i++) {
        Dictionary entry = raw_data[i];
        String species = entry["species"];
        String marker = entry["genetic_marker"];
        
        if (!species_markers.has(species)) {
            species_markers[species] = Array();
        }
        Array markers = species_markers[species];
        markers.push_back(marker);
    }
    
    // Calculate distances between species
    Array species_list = species_markers.keys();
    for (int i = 0; i < species_list.size(); i++) {
        String sp1 = species_list[i];
        result[sp1] = Dictionary();
        
        for (int j = 0; j < species_list.size(); j++) {
            String sp2 = species_list[j];
            Array markers1 = species_markers[sp1];
            Array markers2 = species_markers[sp2];
            
            double distance = calculate_genetic_distance(markers1, markers2);
            result[sp1][sp2] = distance;
        }
    }
    
    return result;
}

DataFormat DataProcessor::detect_format(const String& file_path) const {
    String extension = file_path.get_extension().to_lower();
    
    if (extension == "json") return DataFormat::JSON;
    if (extension == "yaml" || extension == "yml") return DataFormat::YAML;
    if (extension == "toml") return DataFormat::TOML;
    if (extension == "xml") return DataFormat::XML;
    
    // Default to YAML if unknown
    return DataFormat::YAML;
}

Variant DataProcessor::parse_data(const String& content, DataFormat format) {
    switch (format) {
        case DataFormat::JSON: {
            return JSON::parse_string(content);
        }
        
        case DataFormat::YAML: {
            try {
                YAML::Node yaml = YAML::Load(content.utf8().get_data());
                return convert_yaml_to_variant(yaml);
            } catch (const YAML::Exception& e) {
                UtilityFunctions::print("YAML parsing error: ", e.what());
                return Variant();
            }
        }
        
        case DataFormat::TOML: {
            try {
                toml::table tbl = toml::parse(content.utf8().get_data());
                return convert_toml_to_variant(tbl);
            } catch (const toml::parse_error& e) {
                UtilityFunctions::print("TOML parsing error: ", e.what());
                return Variant();
            }
        }
        
        case DataFormat::XML: {
            tinyxml2::XMLDocument doc;
            if (doc.Parse(content.utf8().get_data()) == tinyxml2::XML_SUCCESS) {
                return convert_xml_to_variant(doc.RootElement());
            }
            UtilityFunctions::print("XML parsing error");
            return Variant();
        }
    }
    
    return Variant();
}

String DataProcessor::serialize_data(const Variant& data, DataFormat format) {
    switch (format) {
        case DataFormat::JSON: {
            return JSON::stringify(data);
        }
        
        case DataFormat::YAML: {
            YAML::Node yaml = convert_variant_to_yaml(data);
            YAML::Emitter out;
            out << yaml;
            return String(out.c_str());
        }
        
        case DataFormat::TOML: {
            toml::table tbl = convert_variant_to_toml(data);
            std::stringstream ss;
            ss << tbl;
            return String(ss.str().c_str());
        }
        
        case DataFormat::XML: {
            tinyxml2::XMLDocument doc;
            convert_variant_to_xml(data, doc);
            tinyxml2::XMLPrinter printer;
            doc.Print(&printer);
            return String(printer.CStr());
        }
    }
    
    return String();
}

// Helper conversion functions
Variant DataProcessor::convert_yaml_to_variant(const YAML::Node& node) {
    switch (node.Type()) {
        case YAML::NodeType::Null:
            return Variant();
            
        case YAML::NodeType::Scalar: {
            // Try to parse as different types
            try {
                return node.as<int>();
            } catch (...) {
                try {
                    return node.as<float>();
                } catch (...) {
                    try {
                        return node.as<bool>();
                    } catch (...) {
                        return String(node.as<std::string>().c_str());
                    }
                }
            }
        }
            
        case YAML::NodeType::Sequence: {
            Array arr;
            for (const auto& item : node) {
                arr.push_back(convert_yaml_to_variant(item));
            }
            return arr;
        }
            
        case YAML::NodeType::Map: {
            Dictionary dict;
            for (const auto& it : node) {
                String key = convert_yaml_to_variant(it.first);
                Variant value = convert_yaml_to_variant(it.second);
                dict[key] = value;
            }
            return dict;
        }
            
        default:
            return Variant();
    }
}

// Similar conversion functions for TOML and XML...

Error DataProcessor::load_mod_data(const String& path, DataFormat format) {
    Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
    if (!f.is_valid()) {
        return ERR_FILE_NOT_FOUND;
    }
    
    String content = f->get_as_text();
    Variant parsed = parse_data(content, format);
    
    if (parsed.get_type() == Variant::NIL) {
        return ERR_PARSE_ERROR;
    }
    
    // Store the parsed data
    String category = path.get_basename().get_file().to_lower();
    loaded_data[category] = parsed;
    
    return OK;
}

} 