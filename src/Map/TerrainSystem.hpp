#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <vector>
#include "../Core/SIMDHelper.hpp"

class TerrainSystem {
private:
    struct TerrainChunk {
        alignas(16) std::vector<float> height_data;
        alignas(16) std::vector<float> resource_data;
        alignas(16) std::vector<uint8_t> type_data;
        uint32_t chunk_x, chunk_y;
    };

    static constexpr size_t CHUNK_SIZE = 64;
    static constexpr size_t SIMD_BATCH_SIZE = 8;
    
    std::vector<TerrainChunk> chunks;
    std::vector<float> cached_heights;

public:
    void generate_terrain_simd(size_t width, size_t height) {
        const size_t num_chunks_x = (width + CHUNK_SIZE - 1) / CHUNK_SIZE;
        const size_t num_chunks_y = (height + CHUNK_SIZE - 1) / CHUNK_SIZE;
        
        chunks.resize(num_chunks_x * num_chunks_y);
        
        for (size_t i = 0; i < chunks.size(); i += SIMD_BATCH_SIZE) {
            process_terrain_chunk_simd(&chunks[i], 
                std::min(SIMD_BATCH_SIZE, chunks.size() - i));
        }
    }

private:
    void process_terrain_chunk_simd(TerrainChunk* chunk_batch, size_t count);
}; 