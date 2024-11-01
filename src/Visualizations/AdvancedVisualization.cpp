#include "AdvancedVisualization.hpp"

void AdvancedVisualization::cluster_nodes_parallel(int num_clusters) {
    const size_t point_count = points.size();
    std::vector<float> distances(point_count * num_clusters);
    
    // Parallel clustering using job system
    for (size_t i = 0; i < point_count; i += BATCH_SIZE) {
        job_system->schedule_job([this, i, &distances, num_clusters]() {
            const size_t batch_end = std::min(i + BATCH_SIZE, point_count);
            
            // Calculate distances using SIMD
            calculate_distances_simd(
                std::vector<godot::Vector3>(points.begin() + i, points.begin() + batch_end),
                cluster_centers,
                std::vector<float>(distances.begin() + i * num_clusters, 
                                 distances.begin() + batch_end * num_clusters)
            );
        }, JobSystem::Priority::HIGH);
    }
    
    job_system->process_jobs();
    update_cluster_centers_simd();
}

void AdvancedVisualization::update_cluster_centers_simd() {
    const size_t num_clusters = clusters.size();
    
    // Use SIMD to update cluster centers
    for (size_t i = 0; i < num_clusters; i += 8) {
        const size_t batch_end = std::min(i + 8, num_clusters);
        
        SIMDHelper::update_cluster_centers_simd(
            cluster_centers_x.data() + i,
            cluster_centers_y.data() + i,
            cluster_centers_z.data() + i,
            batch_end - i
        );
    }
} 