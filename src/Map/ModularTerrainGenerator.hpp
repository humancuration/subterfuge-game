#pragma once
#include "TerrainGenerator.hpp"
#include "TerrainSystem.hpp"
#include "../Data/DataProcessor.hpp"
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class ModularTerrainGenerator {
private:
    struct TerrainPreset {
        std::string name;
        std::string description;
        bool enabled = false;
        float weight = 1.0f;
        std::vector<std::string> incompatible_presets;
        std::function<void(ModularTerrainGenerator*)> apply_preset;
        
        // Serialization support for modding
        Dictionary serialize() const {
            Dictionary data;
            data["name"] = name;
            data["description"] = description;
            data["enabled"] = enabled;
            data["weight"] = weight;
            data["incompatible"] = incompatible_presets;
            return data;
        }

        static TerrainPreset from_dictionary(const Dictionary& data) {
            TerrainPreset preset;
            preset.name = data["name"];
            preset.description = data["description"];
            preset.enabled = data["enabled"];
            preset.weight = data["weight"];
            
            Array incompatible = data["incompatible"];
            for (int i = 0; i < incompatible.size(); i++) {
                preset.incompatible_presets.push_back(incompatible[i]);
            }
            
            return preset;
        }
    };

    // SIMD-aligned data for terrain processing
    alignas(32) std::vector<float> elevation_data;
    alignas(32) std::vector<float> moisture_data;
    alignas(32) std::vector<float> temperature_data;
    
    std::unordered_map<std::string, TerrainPreset> presets;
    std::unique_ptr<JobSystem> job_system;
    TerrainGenerator& base_generator;
    std::unique_ptr<Data::DataProcessor> data_processor;

public:
    ModularTerrainGenerator(TerrainGenerator& generator) 
        : base_generator(generator) {
        data_processor = std::make_unique<Data::DataProcessor>();
    }

    // Load presets from configuration files in any supported format
    Error load_presets(const String& preset_path) {
        // Detect format from file extension
        Data::DataFormat format = data_processor->detect_format(preset_path);
        
        // Load and parse the preset file
        Error err = data_processor->load_mod_data(preset_path, format);
        if (err != OK) {
            return err;
        }

        // Get the parsed data
        Dictionary preset_data = data_processor->get_data("terrain_presets", "");
        Array preset_list = preset_data["presets"];

        // Convert to TerrainPreset objects
        for (int i = 0; i < preset_list.size(); i++) {
            Dictionary preset_dict = preset_list[i];
            TerrainPreset preset = TerrainPreset::from_dictionary(preset_dict);
            register_preset(preset);
        }

        return OK;
    }

    // Save presets to file in specified format
    Error save_presets(const String& preset_path, Data::DataFormat format) {
        Dictionary preset_data;
        Array preset_list;

        for (const auto& [name, preset] : presets) {
            preset_list.push_back(preset.serialize());
        }

        preset_data["presets"] = preset_list;
        return data_processor->save_mod_data(preset_path, preset_data, format);
    }

    // Register a new preset programmatically
    void register_preset(const TerrainPreset& preset) {
        presets[preset.name] = preset;
    }

    // Generate terrain with active presets
    void generate_terrain_parallel(TerrainSystem& terrain, size_t width, size_t height) {
        // Process terrain generation in parallel chunks
        const size_t chunk_size = 1024;
        
        for (size_t i = 0; i < width * height; i += chunk_size) {
            job_system->schedule_job(
                [this, i, width, height, chunk_size, &terrain]() {
                    generate_terrain_chunk_simd(i, 
                        std::min(chunk_size, width * height - i),
                        terrain);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
        apply_active_presets(terrain);
    }

    // Get list of available presets
    Array get_available_presets() const {
        Array result;
        for (const auto& [name, preset] : presets) {
            result.push_back(preset.serialize());
        }
        return result;
    }

private:
    void generate_terrain_chunk_simd(size_t start_idx, size_t count, 
                                   TerrainSystem& terrain) {
        // Use existing SIMD helpers for basic terrain generation
        for (size_t i = 0; i < count; i += 8) {
            __m256 elevation = _mm256_load_ps(&elevation_data[start_idx + i]);
            __m256 moisture = _mm256_load_ps(&moisture_data[start_idx + i]);
            __m256 temperature = _mm256_load_ps(&temperature_data[start_idx + i]);
            
            // Apply terrain generation rules using SIMD
            process_terrain_rules_simd(elevation, moisture, temperature);
            
            // Store results
            _mm256_store_ps(&elevation_data[start_idx + i], elevation);
            _mm256_store_ps(&moisture_data[start_idx + i], moisture);
            _mm256_store_ps(&temperature_data[start_idx + i], temperature);
        }
    }

    void apply_active_presets(TerrainSystem& terrain) {
        for (const auto& [name, preset] : presets) {
            if (preset.enabled) {
                preset.apply_preset(this);
            }
        }
    }
}; 