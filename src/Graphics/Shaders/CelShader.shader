shader_type spatial;

uniform vec4 outline_color : hint_color = vec4(0.0, 0.0, 0.0, 1.0);
uniform float outline_thickness = 0.05;

void vertex() {
    // Modify vertex positions for the outline
    // This is a simplified example; actual outline implementation may vary
}

void fragment() {
    // Simple cel shading: quantize lighting
    float intensity = dot(NORMAL, normalize(vec3(0.5, 1.0, 0.3)));
    if (intensity > 0.95)
        ALBEDO = vec3(1.0, 1.0, 1.0);
    else if (intensity > 0.5)
        ALBEDO = vec3(0.7, 0.7, 0.7);
    else
        ALBEDO = vec3(0.3, 0.3, 0.3);
}