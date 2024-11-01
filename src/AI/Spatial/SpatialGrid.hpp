#pragma once
#include <vector>
#include <unordered_map>
#include "../NPCController.hpp"
#include <memory>

class SpatialGrid {
private:
    struct Cell {
        std::vector<std::weak_ptr<NPCController>> npcs;
        std::mutex mutex;
    };

    float cellSize;
    std::unordered_map<int64_t, Cell> grid;
    std::mutex grid_mutex;

    static int64_t get_cell_key(int x, int y) {
        return (static_cast<int64_t>(x) << 32) | static_cast<uint32_t>(y);
    }

public:
    explicit SpatialGrid(float cell_size = 100.0f) : cellSize(cell_size) {}

    void update_npc_position(const std::shared_ptr<NPCController>& npc, float x, float y) {
        int cell_x = static_cast<int>(x / cellSize);
        int cell_y = static_cast<int>(y / cellSize);
        int64_t key = get_cell_key(cell_x, cell_y);

        std::lock_guard<std::mutex> lock(grid_mutex);
        grid[key].npcs.push_back(npc);
    }

    std::vector<std::shared_ptr<NPCController>> get_nearby_npcs(float x, float y, float radius) {
        std::vector<std::shared_ptr<NPCController>> result;
        int cell_radius = static_cast<int>(radius / cellSize) + 1;
        int center_x = static_cast<int>(x / cellSize);
        int center_y = static_cast<int>(y / cellSize);

        for (int dx = -cell_radius; dx <= cell_radius; ++dx) {
            for (int dy = -cell_radius; dy <= cell_radius; ++dy) {
                int64_t key = get_cell_key(center_x + dx, center_y + dy);
                auto it = grid.find(key);
                if (it != grid.end()) {
                    std::lock_guard<std::mutex> lock(it->second.mutex);
                    for (auto& weak_npc : it->second.npcs) {
                        if (auto npc = weak_npc.lock()) {
                            result.push_back(npc);
                        }
                    }
                }
            }
        }
        return result;
    }
}; 