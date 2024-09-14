shader_type spatial;

uniform float time : hint_range(0, 100) = 0.0;
uniform vec4 ripple_color : hint_color = vec4(0.0, 0.5, 1.0, 0.5);
uniform float wave_speed = 1.0;
uniform float wave_frequency = 10.0;

void vertex() {
    VERTEX.y += sin((UV.x + time * wave_speed) * wave_frequency) * 0.1;
}

void fragment() {
    ALBEDO = ripple_color.rgb;
    ALPHA = ripple_color.a;
}