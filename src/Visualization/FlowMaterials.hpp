#pragma once
#include <godot_cpp/classes/shadermaterial.hpp>
#include <godot_cpp/classes/noise_texture.hpp>

class FlowMaterials {
public:
    static godot::Ref<godot::ShaderMaterial> create_flow_material(
        const std::string& type,
        float intensity
    ) {
        auto material = godot::Ref<godot::ShaderMaterial>(memnew(godot::ShaderMaterial));
        
        // Load appropriate shader
        if (type == "knowledge") {
            material->set_shader(load_knowledge_shader());
            setup_knowledge_material(material, intensity);
        } else {
            material->set_shader(load_flow_shader());
            setup_flow_material(material, type, intensity);
        }
        
        return material;
    }

private:
    static void setup_flow_material(
        godot::Ref<godot::ShaderMaterial> material,
        const std::string& type,
        float intensity
    ) {
        // Set base parameters
        material->set_shader_parameter("flow_speed", 1.0f + intensity);
        material->set_shader_parameter("pulse_intensity", 0.3f + intensity * 0.2f);
        material->set_shader_parameter("glow_strength", intensity);
        
        // Set color based on type
        godot::Color color;
        if (type == "resource") {
            color = godot::Color(0.4f, 0.7f, 1.0f); // Blue
        } else if (type == "cultural") {
            color = godot::Color(1.0f, 0.4f, 0.7f); // Pink
        }
        material->set_shader_parameter("flow_color", color);
        
        // Create and set noise texture
        auto noise = create_flow_noise();
        material->set_shader_parameter("noise_texture", noise);
    }

    static void setup_knowledge_material(
        godot::Ref<godot::ShaderMaterial> material,
        float intensity
    ) {
        material->set_shader_parameter("flow_speed", 0.5f + intensity);
        material->set_shader_parameter("particle_density", 0.3f + intensity * 0.4f);
        material->set_shader_parameter("energy_level", intensity);
        
        // Set gradient colors
        material->set_shader_parameter("primary_color", 
            godot::Color(0.4f, 0.7f, 1.0f));
        material->set_shader_parameter("secondary_color", 
            godot::Color(0.4f, 1.0f, 0.7f));
        
        // Create and set pattern texture
        auto pattern = create_knowledge_pattern();
        material->set_shader_parameter("pattern_texture", pattern);
    }

    static godot::Ref<godot::NoiseTexture> create_flow_noise() {
        auto noise = godot::Ref<godot::NoiseTexture>(memnew(godot::NoiseTexture));
        // Configure noise parameters
        return noise;
    }

    static godot::Ref<godot::NoiseTexture> create_knowledge_pattern() {
        auto pattern = godot::Ref<godot::NoiseTexture>(memnew(godot::NoiseTexture));
        // Configure pattern parameters
        return pattern;
    }
}; 