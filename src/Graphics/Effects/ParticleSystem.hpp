#pragma once
#include <godot_cpp/classes/cpu_particles3d.hpp>
#include "../Core/SIMDHelper.hpp"
#include <vector>

class ParticleSystem : public godot::CPUParticles3D {
    GDCLASS(ParticleSystem, CPUParticles3D)

private:
    struct Particle {
        alignas(16) godot::Vector3 position;
        alignas(16) godot::Vector3 velocity;
        float lifetime;
        float size;
    };

    // SIMD-aligned particle data
    alignas(32) std::vector<float> positions_x;
    alignas(32) std::vector<float> positions_y;
    alignas(32) std::vector<float> positions_z;
    alignas(32) std::vector<float> velocities_x;
    alignas(32) std::vector<float> velocities_y;
    alignas(32) std::vector<float> velocities_z;

    static constexpr size_t SIMD_BATCH_SIZE = 8;
    std::vector<Particle> particles;

protected:
    static void _bind_methods();

public:
    ParticleSystem();
    void update_particles_simd(float delta);
    void emit_particle(const godot::Vector3& position, const godot::Vector3& velocity);
}; 