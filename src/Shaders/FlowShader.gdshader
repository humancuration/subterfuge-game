shader_type spatial;

// Parameters that can be set from code
uniform vec4 flow_color : source_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform float flow_speed : hint_range(0.1, 5.0) = 1.0;
uniform float pulse_intensity : hint_range(0.0, 1.0) = 0.5;
uniform float glow_strength : hint_range(0.0, 2.0) = 1.0;
uniform float flow_progress : hint_range(0.0, 1.0) = 0.0;
uniform sampler2D noise_texture;

void vertex() {
    // Add subtle wave motion to the path
    float wave = sin(TIME * flow_speed + VERTEX.x) * 0.02;
    VERTEX.y += wave;
}

void fragment() {
    // Flow animation using noise
    vec2 noise_uv = vec2(
        UV.x - TIME * flow_speed,
        UV.y
    );
    float noise = texture(noise_texture, noise_uv).r;
    
    // Create flowing effect
    float flow_mask = smoothstep(
        flow_progress - 0.1,
        flow_progress + 0.1,
        UV.x
    );
    
    // Pulse effect
    float pulse = sin(TIME * 2.0) * 0.5 + 0.5;
    pulse = mix(1.0, pulse, pulse_intensity);
    
    // Edge glow
    float edge = 1.0 - abs(UV.y - 0.5) * 2.0;
    edge = pow(edge, 3.0);
    
    // Combine effects
    vec4 final_color = flow_color;
    final_color.a *= flow_mask * edge * pulse;
    final_color.rgb += glow_strength * edge * pulse;
    
    // Add noise detail
    final_color.a *= mix(0.8, 1.0, noise);
    
    ALBEDO = final_color.rgb;
    ALPHA = final_color.a;
    EMISSION = final_color.rgb * glow_strength * pulse;
} 