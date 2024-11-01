#pragma once
#include <vector>
#include <unordered_map>
#include "../Core/ObjectPool.hpp"
#include "City.hpp"

class SpatialPartitioningSystem {
private:
    struct GridCell {
        alignas(16) std::vector<City*> cities;
        alignas(16) std::vector<float> distances;
        uint32_t x, y;
    };

    float cell_size;
    std::vector<GridCell> grid;
    ObjectPool<GridCell> cell_pool;
    
    static constexpr size_t CELL_CAPACITY = 16;

public:
    explicit SpatialPartitioningSystem(float cell_size = 100.0f) 
        : cell_size(cell_size) {}

    void insert_city(City* city) {
        auto [cell_x, cell_y] = get_cell_coords(city->get_position());
        auto& cell = get_or_create_cell(cell_x, cell_y);
        
        if (cell.cities.size() < CELL_CAPACITY) {
            cell.cities.push_back(city);
            update_cell_distances_simd(cell);
        }
    }

    std::vector<City*> query_radius(const godot::Vector2& position, float radius) {
        std::vector<City*> result;
        auto [min_x, min_y] = get_cell_coords(position - godot::Vector2(radius, radius));
        auto [max_x, max_y] = get_cell_coords(position + godot::Vector2(radius, radius));
        
        for (uint32_t y = min_y; y <= max_y; ++y) {
            for (uint32_t x = min_x; x <= max_x; ++x) {
                if (auto cell = get_cell(x, y)) {
                    query_cell_simd(*cell, position, radius, result);
                }
            }
        }
        
        return result;
    }

private:
    void update_cell_distances_simd(GridCell& cell);
    void query_cell_simd(const GridCell& cell, const godot::Vector2& position, 
                        float radius, std::vector<City*>& result);
}; 