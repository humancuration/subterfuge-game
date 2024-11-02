#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <vector>
#include <memory>

namespace Core {

class NodeStats : public godot::Resource {
    GDCLASS(NodeStats, Resource)
    // Add your node stats properties here
};

class NodeData : public godot::Resource {
    GDCLASS(NodeData, Resource)

private:
    int id;
    String name;
    Ref<NodeStats> stats;
    Vector<int> connected_node_ids;

protected:
    static void _bind_methods();

public:
    NodeData();
    // Add getters/setters
};

class NPCData : public godot::Resource {
    GDCLASS(NPCData, Resource)

private:
    int id;
    String name;
    int current_node_id;
    Vector<String> traits;

protected:
    static void _bind_methods();

public:
    NPCData();
    // Add getters/setters
};

class GameState : public godot::Resource {
    GDCLASS(GameState, Resource)

private:
    Vector<Ref<NPCData>> npcs;
    Vector<Ref<NodeData>> nodes;

protected:
    static void _bind_methods();

public:
    GameState();
    
    void add_npc(const Ref<NPCData>& npc);
    void add_node(const Ref<NodeData>& node);
    
    Vector<Ref<NPCData>> get_npcs() const { return npcs; }
    Vector<Ref<NodeData>> get_nodes() const { return nodes; }
};

} 