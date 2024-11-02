#include "DataLoader.hpp"
#include <JSONParseResult.hpp>
#include <Array.hpp>
#include <Dictionary.hpp>

namespace Systems {

using namespace godot;

void DataLoader::_register_methods() {
    register_method("load_election_definitions", &DataLoader::load_election_definitions);
    register_method("load_candidate_definitions", &DataLoader::load_candidate_definitions);
}

DataLoader::DataLoader() {}
DataLoader::~DataLoader() {}

void DataLoader::_init() {}

godot::Error DataLoader::load_json_file(const godot::String& path, godot::String& json_content) {
    godot::String full_path = ProjectSettings::get_singleton()->globalize_path(path);
    
    Ref<File> file;
    file.instance();
    
    Error err = file->open(full_path, File::READ);
    if (err != Error::OK) {
        Godot::print_error("File not found: " + path, "load_json_file", __FILE__, __LINE__);
        return err;
    }

    json_content = file->get_as_text();
    file->close();
    
    return Error::OK;
}

std::vector<Election*> DataLoader::load_election_definitions(const godot::String& path) {
    std::vector<Election*> elections;
    godot::String json_content;
    
    if (load_json_file(path, json_content) != Error::OK) {
        return elections;
    }

    JSON json;
    Ref<JSONParseResult> result = json.parse(json_content);
    
    if (result->get_error() != Error::OK) {
        Godot::print_error("Failed to parse election definitions", 
                          "load_election_definitions",
                          __FILE__, __LINE__);
        return elections;
    }

    Dictionary data = result->get_result();
    Array election_data = data["elections"];

    for (int i = 0; i < election_data.size(); i++) {
        Dictionary election_dict = election_data[i];
        
        Election* election = Election::_new();
        election->set_title(election_dict["title"]);
        election->set_frequency(static_cast<int>(election_dict["frequency_in_years"]));
        
        // Parse applicable regions
        Array regions = election_dict["applicable_regions"];
        std::vector<String> applicable_regions;
        for (int j = 0; j < regions.size(); j++) {
            applicable_regions.push_back(regions[j]);
        }
        
        elections.push_back(election);
    }

    return elections;
}

std::vector<Candidate*> DataLoader::load_candidate_definitions(const godot::String& path) {
    std::vector<Candidate*> candidates;
    godot::String json_content;
    
    if (load_json_file(path, json_content) != Error::OK) {
        return candidates;
    }

    JSON json;
    Ref<JSONParseResult> result = json.parse(json_content);
    
    if (result->get_error() != Error::OK) {
        Godot::print_error("Failed to parse candidate definitions", 
                          "load_candidate_definitions",
                          __FILE__, __LINE__);
        return candidates;
    }

    Dictionary data = result->get_result();
    Array candidate_data = data["candidates"];

    for (int i = 0; i < candidate_data.size(); i++) {
        Dictionary candidate_dict = candidate_data[i];
        
        Candidate* candidate = Candidate::_new();
        candidate->set_name(candidate_dict["name"]);
        
        // Parse platform
        Dictionary platform = candidate_dict["platform"];
        Array keys = platform.keys();
        for (int j = 0; j < keys.size(); j++) {
            String key = keys[j];
            float value = platform[key];
            candidate->add_platform_value(key, value);
        }
        
        candidates.push_back(candidate);
    }

    return candidates;
}

} // namespace Systems 