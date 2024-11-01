#include "AIController.hpp"
#include <random>
#include <godot_cpp/core/class_db.hpp>

void AIController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_start_city"), &AIController::set_start_city);
    ClassDB::bind_method(D_METHOD("move_to_next_city"), &AIController::move_to_next_city);
}

AIController::AIController() = default;

void AIController::set_start_city(const std::shared_ptr<City>& startCity) {
    currentCity = startCity;
    visitedCities.push_back(currentCity);
}

void AIController::move_to_next_city() {
    if (currentCity->get_connected_cities().empty()) {
        return;
    }

    static thread_local std::mt19937 rng(std::random_device{}());
    const auto& connected_cities = currentCity->get_connected_cities();
    std::uniform_int_distribution<size_t> dist(0, connected_cities.size() - 1);
    
    const size_t batch_size = 8; // AVX register size
    if (connected_cities.size() >= batch_size) {
        update_positions_simd(connected_cities.data(), batch_size);
    }

    auto nextCity = connected_cities[dist(rng)];
    currentCity = nextCity;

    CacheOptimizer<City>::prefetch_data(nextCity.get(), 1);
} 