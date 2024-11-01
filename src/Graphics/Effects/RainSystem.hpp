#pragma once
#include "ParticleSystem.hpp"

class RainSystem : public ParticleSystem {
    GDCLASS(RainSystem, ParticleSystem)

private:
    godot::Vector3 wind_direction;
    float wind_strength{0.0f};
    float rain_intensity{1.0f};

    // Pre-calculated constants for SIMD
    alignas(32) float gravity_constants[8] = {-9.81f, -9.81f, -9.81f, -9.81f, -9.81f, -9.81f, -9.81f, -9.81f};

public:
    void set_wind(const godot::Vector3& direction, float strength);
    void update_rain_simd(float delta);
    void generate_rain_batch();
}; 