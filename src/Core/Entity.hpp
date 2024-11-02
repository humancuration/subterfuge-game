#pragma once
#include <godot_cpp/classes/node.hpp>
#include <string>

class Entity : public godot::Node {
    GDCLASS(Entity, Node)

protected:
    int id;
    String name;

    static void _bind_methods();

public:
    Entity();
    explicit Entity(int p_id);

    void set_id(int p_id) { id = p_id; }
    int get_id() const { return id; }
    
    void set_name(const String& p_name) { name = p_name; }
    String get_name() const { return name; }
}; 