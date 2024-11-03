#pragma once
#include <string>
#include <vector>
#include <memory>

class Voter {
public:
    struct Opinion {
        std::string topic;
        float value{0.0f};  // -1.0 to 1.0
        float confidence{0.5f};
        float influence_resistance{0.0f};
    };

private:
    std::string id;
    std::vector<Opinion> opinions;
    float overall_satisfaction{0.5f};
    bool has_voted{false};

public:
    explicit Voter(const std::string& voter_id) : id(voter_id) {}

    const std::string& get_id() const { return id; }

    void add_opinion(const std::string& topic, float initial_value) {
        opinions.push_back({topic, initial_value});
    }

    void modify_opinion(float delta) {
        for (auto& opinion : opinions) {
            float resistance = opinion.influence_resistance;
            float actual_delta = delta * (1.0f - resistance);
            opinion.value = std::clamp(opinion.value + actual_delta, -1.0f, 1.0f);
        }
        update_satisfaction();
    }

    float get_opinion(const std::string& topic) const {
        auto it = std::find_if(opinions.begin(), opinions.end(),
            [&topic](const Opinion& o) { return o.topic == topic; });
        return it != opinions.end() ? it->value : 0.0f;
    }

    void cast_vote() {
        has_voted = true;
    }

    bool has_cast_vote() const {
        return has_voted;
    }

    float get_satisfaction() const {
        return overall_satisfaction;
    }

private:
    void update_satisfaction() {
        float total = 0.0f;
        for (const auto& opinion : opinions) {
            total += opinion.value * opinion.confidence;
        }
        overall_satisfaction = (total / opinions.size() + 1.0f) / 2.0f;
    }
}; 