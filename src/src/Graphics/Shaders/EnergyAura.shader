shader_type spatial;

uniform vec4 aura_color : hint_color = vec4(0.0, 1.0, 1.0, 1.0);
uniform float max_distance = 2.0;
uniform float flow_speed = 1.0;

void fragment() {
    float distance = length(FRAGCOORD.xy - vec2(0.5));
    float alpha = 1.0 - smoothstep(max_distance - 0.5, max_distance + 0.5, distance);
    ALBEDO = aura_color.rgb;
    ALPHA = alpha * aura_color.a;
}