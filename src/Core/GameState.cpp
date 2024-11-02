#include "GameState.hpp"

namespace Core {

void NodeData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &NodeData::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &NodeData::get_id);
    ClassDB::bind_method(D_METHOD("set_name", "p_name"), &NodeData::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &NodeData::get_name);
    ClassDB::bind_method(D_METHOD("set_stats", "p_stats"), &NodeData::set_stats);
    ClassDB::bind_method(D_METHOD("get_stats"), &NodeData::get_stats);
    ClassDB::bind_method(D_METHOD("get_connected_node_ids"), &NodeData::get_connected_node_ids);
    ClassDB::bind_method(D_METHOD("add_connected_node", "node_id"), &NodeData::add_connected_node);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stats", PROPERTY_HINT_RESOURCE_TYPE, "NodeStats"), "set_stats", "get_stats");
}

NodeData::NodeData() : id(0) {}

void NPCData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &NPCData::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &NPCData::get_id);
    ClassDB::bind_method(D_METHOD("set_name", "p_name"), &NPCData::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &NPCData::get_name);
    ClassDB::bind_method(D_METHOD("set_current_node_id", "p_node_id"), &NPCData::set_current_node_id);
    ClassDB::bind_method(D_METHOD("get_current_node_id"), &NPCData::get_current_node_id);
    ClassDB::bind_method(D_METHOD("get_traits"), &NPCData::get_traits);
    ClassDB::bind_method(D_METHOD("add_trait", "trait"), &NPCData::add_trait);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_node_id"), "set_current_node_id", "get_current_node_id");
}

NPCData::NPCData() : id(0), current_node_id(0) {}

void GameState::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_npc", "npc"), &GameState::add_npc);
    ClassDB::bind_method(D_METHOD("add_node", "node"), &GameState::add_node);
    ClassDB::bind_method(D_METHOD("get_npcs"), &GameState::get_npcs);
    ClassDB::bind_method(D_METHOD("get_nodes"), &GameState::get_nodes);
}

GameState::GameState() {}

void GameState::add_npc(const Ref<NPCData>& npc) {
    if (npc.is_valid()) {
        npcs.push_back(npc);
    }
}

void GameState::add_node(const Ref<NodeData>& node) {
    if (node.is_valid()) {
        nodes.push_back(node);
    }
}

} 