shader_type spatial;

uniform vec4 primary_color : source_color;
uniform vec4 secondary_color : source_color;
uniform float flow_speed : hint_range(0.1, 5.0) = 1.0;
uniform float particle_density : hint_range(0.0, 1.0) = 0.5;
uniform float energy_level : hint_range(0.0, 1.0) = 1.0;
uniform sampler2D pattern_texture;

void vertex() {
    // Add floating motion
    float hover = sin(TIME * 2.0 + VERTEX.x * 2.0) * 0.01;
    VERTEX.y += hover;
}

void fragment() {
    // Flowing pattern animation
    vec2 pattern_uv = vec2(
        UV.x - TIME * flow_speed,
        UV.y + sin(UV.x * 10.0 + TIME) * 0.1
    );
    float pattern = texture(pattern_texture, pattern_uv).r;
    
    // Create particle effect
    float particles = step(1.0 - particle_density, pattern);
    
    // Color gradient animation
    float gradient = sin(UV.x * 3.1415 + TIME * flow_speed) * 0.5 + 0.5;
    vec4 color = mix(primary_color, secondary_color, gradient);
    
    // Energy pulse
    float pulse = sin(TIME * 3.0) * 0.5 + 0.5;
    pulse = mix(0.8, 1.0, pulse * energy_level);
    
    // Edge glow
    float edge = 1.0 - abs(UV.y - 0.5) * 2.0;
    edge = pow(edge, 2.0);
    
    // Final composition
    vec4 final_color = color;
    final_color.a *= particles * edge * pulse;
    final_color.rgb += edge * energy_level * 0.5;
    
    ALBEDO = final_color.rgb;
    ALPHA = final_color.a;
    EMISSION = final_color.rgb * energy_level * pulse;
} 