#include "Entity.hpp"

void Entity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &Entity::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &Entity::get_id);
    ClassDB::bind_method(D_METHOD("set_name", "p_name"), &Entity::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &Entity::get_name);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "entity_name"), "set_name", "get_name");
}

Entity::Entity() : id(0) {}

Entity::Entity(int p_id) : id(p_id) {} 