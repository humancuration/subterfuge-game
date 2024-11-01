#pragma once
#include <vector>
#include <memory>
#include "City.hpp"
#include <godot_cpp/classes/node.hpp>

class AIController : public godot::Node {
    GDCLASS(AIController, Node)

private:
    std::shared_ptr<City> currentCity;
    std::vector<std::shared_ptr<City>> visitedCities;

protected:
    static void _bind_methods();

public:
    void set_start_city(const std::shared_ptr<City>& startCity);
    void move_to_next_city();
    AIController();
    ~AIController() = default;
}; 