#ifndef REGION_HPP
#define REGION_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <memory>
#include "election.hpp"
#include "voter.hpp"
#include "node_stats.hpp"

namespace game_systems {

class Region : public godot::Node {
    GDCLASS(Region, Node)

private:
    String name;
    NodeStats stats;
    std::vector<std::unique_ptr<Election>> elections;
    std::vector<std::unique_ptr<Voter>> voters;

    void initialize_voters();
    void conduct_election(Election* election);
    std::vector<Candidate> load_candidates(ElectionType type);

protected:
    static void _bind_methods();

public:
    Region();
    explicit Region(const String& p_name);
    ~Region() = default;

    void initialize_elections();
    void handle_elections(double delta);

    // Getters/Setters
    void set_name(const String& p_name) { name = p_name; }
    String get_name() const { return name; }
    NodeStats* get_stats() { return &stats; }
    
    const std::vector<std::unique_ptr<Voter>>& get_voters() const { return voters; }
};

}  // namespace game_systems

#endif // REGION_HPP 