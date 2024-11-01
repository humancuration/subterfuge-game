#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include "../Core/JobSystem.hpp"
#include "../Core/SIMDHelper.hpp"
#include <vector>

class AdvancedVisualization : public godot::Node3D {
    GDCLASS(AdvancedVisualization, Node3D)

private:
    struct Cluster {
        std::vector<godot::Vector3> points;
        godot::Color color;
    };

    std::unique_ptr<JobSystem> job_system;
    std::vector<Cluster> clusters;
    
    // SIMD-aligned data for clustering
    alignas(32) std::vector<float> cluster_centers_x;
    alignas(32) std::vector<float> cluster_centers_y;
    alignas(32) std::vector<float> cluster_centers_z;

public:
    void cluster_nodes_parallel(int num_clusters);
    void update_cluster_centers_simd();
    void highlight_cluster(size_t cluster_index);

private:
    void calculate_distances_simd(const std::vector<godot::Vector3>& points, 
                                const std::vector<godot::Vector3>& centers,
                                std::vector<float>& distances);
}; 