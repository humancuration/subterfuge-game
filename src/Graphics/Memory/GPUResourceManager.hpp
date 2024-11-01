#pragma once
#include <godot_cpp/classes/rd_texture.hpp>
#include <godot_cpp/classes/rd_buffer.hpp>
#include <queue>
#include <memory>

class GPUResourceManager {
private:
    struct GPUResource {
        uint64_t last_used_frame;
        size_t size_bytes;
        void* resource_ptr;
    };

    // Resource pools
    std::vector<std::unique_ptr<godot::RDTexture>> texture_pool;
    std::vector<std::unique_ptr<godot::RDBuffer>> buffer_pool;
    
    // Resource tracking
    std::unordered_map<uint32_t, GPUResource> active_resources;
    std::queue<uint32_t> available_resource_ids;
    
    size_t total_memory_usage{0};
    size_t memory_budget{1024 * 1024 * 1024}; // 1GB default

public:
    uint32_t allocate_texture(const godot::Vector2i& size, godot::RDTextureFormat format);
    uint32_t allocate_buffer(size_t size_bytes, godot::RDBufferUsageBits usage);
    void release_resource(uint32_t resource_id);
    void garbage_collect(uint64_t current_frame);

private:
    void evict_least_recently_used();
    void defragment_memory();
}; 