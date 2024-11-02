#ifndef RANDOMEVENT_SYSTEM_HPP
#define RANDOMEVENT_SYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <vector>
#include "ISystem.hpp"
#include "../Events/GameEvent.hpp"

namespace Systems {

class RandomEventSystem : public godot::Node, public ISystem {
    GODOT_CLASS(RandomEventSystem, godot::Node)

private:
    std::vector<GameEvent*> possible_events;
    godot::Ref<godot::RandomNumberGenerator> rng;

public:
    static void _register_methods();
    
    RandomEventSystem();
    ~RandomEventSystem();

    void _init();
    void update(float delta_time) override;

private:
    void initialize_possible_events();
    bool should_trigger_event();
    GameEvent* select_random_event();
};

} // namespace Systems

#endif // RANDOMEVENT_SYSTEM_HPP 