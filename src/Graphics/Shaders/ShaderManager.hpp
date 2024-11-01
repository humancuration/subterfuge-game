#pragma once
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ShaderManager {
private:
    static ShaderManager* instance;
    std::unordered_map<std::string, godot::Ref<godot::Shader>> shader_cache;
    std::unordered_map<std::string, godot::Ref<godot::ShaderMaterial>> material_cache;

    // Shader uniform buffer objects for efficient updates
    struct UniformBuffer {
        alignas(16) float time;
        alignas(16) godot::Vector4 color;
        alignas(16) godot::Vector4 parameters;
    };

    std::vector<UniformBuffer> uniform_buffers;

public:
    static ShaderManager& get_instance();
    
    godot::Ref<godot::ShaderMaterial> get_material(const std::string& name);
    void update_shader_parameters_simd(float delta);
    void batch_update_materials();
}; 