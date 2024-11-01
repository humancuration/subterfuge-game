#pragma once
#include <string>
#include <memory>
#include "AIController.hpp"
#include "States/INPCState.hpp"
#include "WeatherCondition.hpp"
#include <godot_cpp/classes/node.hpp>

class NPCController : public godot::Node {
    GDCLASS(NPCController, Node)

private:
    std::string name;
    std::unique_ptr<AIController> aiController;
    std::unique_ptr<INPCState> currentState;
    float defense{0.0f};
    float corporateAlignment{0.0f};
    float publicAlignment{0.0f};

protected:
    static void _bind_methods();

public:
    NPCController();
    explicit NPCController(std::string npcName, std::unique_ptr<AIController> controller);
    
    void update(float deltaTime);
    void change_state(std::unique_ptr<INPCState> newState);
    void react_to_weather(WeatherCondition weather);
    void decide_next_action();
    
    // Getters and setters
    const std::string& get_name() const { return name; }
    void set_defense(float value) { defense = value; }
    float get_defense() const { return defense; }
}; 