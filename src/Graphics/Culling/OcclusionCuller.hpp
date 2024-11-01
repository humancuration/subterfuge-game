#pragma once
#include <godot_cpp/classes/camera3d.hpp>
#include "../Core/JobSystem.hpp"
#include <vector>

class OcclusionCuller {
private:
    struct OccluderData {
        alignas(16) godot::AABB bounds;
        float importance;
        bool is_visible;
    };

    // SIMD-aligned visibility data
    alignas(32) std::vector<float> depth_values;
    alignas(32) std::vector<float> screen_coords_x;
    alignas(32) std::vector<float> screen_coords_y;
    
    std::vector<OccluderData> occluders;
    std::unique_ptr<JobSystem> job_system;

public:
    void update_visibility(const godot::Camera3D* camera);
    void process_occlusion_simd(size_t start_idx, size_t count);
    void optimize_occluder_hierarchy();
}; 