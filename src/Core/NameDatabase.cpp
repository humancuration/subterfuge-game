#include "NameDatabase.hpp"
#include <godot_cpp/classes/file_access.hpp>

void NameDatabase::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_from_file", "file_path"), &NameDatabase::load_from_file);
    ClassDB::bind_method(D_METHOD("get_random_npc_name"), &NameDatabase::get_random_npc_name);
    ClassDB::bind_method(D_METHOD("get_random_node_name"), &NameDatabase::get_random_node_name);
    ClassDB::bind_method(D_METHOD("get_random_corporation_name"), &NameDatabase::get_random_corporation_name);
    ClassDB::bind_method(D_METHOD("get_random_group_name"), &NameDatabase::get_random_group_name);
    ClassDB::bind_method(D_METHOD("get_random_culture_name"), &NameDatabase::get_random_culture_name);
}

NameDatabase::NameDatabase() : gen(rd()) {}

void NameDatabase::load_from_file(const String& file_path) {
    Ref<FileAccess> f = FileAccess::open(file_path, FileAccess::READ);
    if (f.is_valid()) {
        String json_str = f->get_as_text();
        Variant parsed = JSON::parse_string(json_str);
        
        if (parsed.get_type() == Variant::DICTIONARY) {
            Dictionary data = parsed;
            
            Dictionary npc_names = data.get("npc_names", Dictionary());
            npc_first_names = npc_names.get("first_names", Array());
            npc_last_names = npc_names.get("last_names", Array());
            
            node_names = data.get("node_names", Array());
            corporation_names = data.get("corporation_names", Array());
            group_names = data.get("group_names", Array());
            culture_names = data.get("culture_names", Array());
        }
    }
}

String NameDatabase::get_random_npc_name() const {
    String first_name = get_random_element(npc_first_names);
    String last_name = get_random_element(npc_last_names);
    return first_name + " " + last_name;
}

String NameDatabase::get_random_node_name() const {
    return get_random_element(node_names);
}

String NameDatabase::get_random_corporation_name() const {
    return get_random_element(corporation_names);
}

String NameDatabase::get_random_group_name() const {
    return get_random_element(group_names);
}

String NameDatabase::get_random_culture_name() const {
    return get_random_element(culture_names);
}

String NameDatabase::get_random_element(const Array& arr) const {
    if (arr.size() == 0) {
        return "";
    }
    std::uniform_int_distribution<> dis(0, arr.size() - 1);
    return arr[dis(gen)];
} 