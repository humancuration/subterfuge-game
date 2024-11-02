#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/json.hpp>
#include <random>

class NameDatabase : public godot::Resource {
    GDCLASS(NameDatabase, Resource)

private:
    Array npc_first_names;
    Array npc_last_names;
    Array node_names;
    Array corporation_names;
    Array group_names;
    Array culture_names;
    
    std::random_device rd;
    std::mt19937 gen;

protected:
    static void _bind_methods();

public:
    NameDatabase();
    void load_from_file(const String& file_path);
    
    String get_random_npc_name() const;
    String get_random_node_name() const;
    String get_random_corporation_name() const;
    String get_random_group_name() const;
    String get_random_culture_name() const;

private:
    String get_random_element(const Array& arr) const;
}; 