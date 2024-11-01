#pragma once
#include <godot_cpp/classes/directional_light3d.hpp>
#include <vector>
#include "../Core/JobSystem.hpp"

class LightManager {
private:
    struct ShadowData {
        alignas(16) godot::Vector3 position;
        alignas(16) godot::Vector3 direction;
        float intensity;
        int resolution;
    };

    std::vector<godot::DirectionalLight3D*> lights;
    std::unique_ptr<JobSystem> job_system;
    
    // SIMD-aligned shadow calculation data
    alignas(32) std::vector<float> shadow_intensities;
    alignas(32) std::vector<float> light_positions_x;
    alignas(32) std::vector<float> light_positions_y;
    alignas(32) std::vector<float> light_positions_z;

public:
    void update_shadows_parallel();
    void calculate_shadow_batch_simd(size_t start_index, size_t count);
    void optimize_shadow_cascades();
}; 