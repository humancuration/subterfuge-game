#pragma once
#include <godot_cpp/classes/node.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../Systems/Voter.hpp"
#include "../Systems/TimeSystem.hpp"

class VotingInfluencer : public godot::Node {
    GDCLASS(VotingInfluencer, Node)

private:
    struct Influence {
        std::string source_id;
        std::string target_id;
        float strength{0.0f};
        float duration{0.0f};
        bool is_positive{true};
    };

    std::vector<Influence> active_influences;
    std::vector<std::weak_ptr<Voter>> affected_voters;
    TimeSystem* time_system{nullptr};

public:
    void add_influence(const std::string& source_id, 
                      const std::string& target_id,
                      float strength,
                      float duration,
                      bool is_positive = true) {
        active_influences.push_back({
            source_id,
            target_id,
            strength,
            duration,
            is_positive
        });
    }

    void register_voter(std::shared_ptr<Voter> voter) {
        affected_voters.push_back(voter);
    }

    void _ready() override {
        time_system = get_node<TimeSystem>("../TimeSystem");
    }

    void _process(float delta) override {
        update_influences(delta);
        apply_influences();
    }

private:
    void update_influences(float delta) {
        auto it = active_influences.begin();
        while (it != active_influences.end()) {
            it->duration -= delta;
            if (it->duration <= 0) {
                it = active_influences.erase(it);
            } else {
                ++it;
            }
        }
    }

    void apply_influences() {
        // Clean up expired voter references
        affected_voters.erase(
            std::remove_if(affected_voters.begin(), affected_voters.end(),
                [](const auto& weak_voter) { return weak_voter.expired(); }),
            affected_voters.end()
        );

        // Apply influences to remaining voters
        for (const auto& weak_voter : affected_voters) {
            if (auto voter = weak_voter.lock()) {
                for (const auto& influence : active_influences) {
                    if (voter->get_id() == influence.target_id) {
                        float modifier = influence.is_positive ? 1.0f : -1.0f;
                        voter->modify_opinion(influence.strength * modifier);
                    }
                }
            }
        }
    }

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("add_influence", "source_id", "target_id", 
                                    "strength", "duration", "is_positive"),
                           &VotingInfluencer::add_influence);
    }
}; 