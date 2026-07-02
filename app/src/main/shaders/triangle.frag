#version 450
layout(location = 0) out vec4 outColor;
layout(push_constant) uniform PushConstants {
    int load_factor;
} push;

void main() {
    vec2 uv = gl_FragCoord.xy / 1000.0;
    vec4 color = vec4(uv.x, uv.y, 0.5, 1.0);
    int iterations = push.load_factor * 1000;
    // Waste some GPU cycles based on load_factor
    for (int i = 0; i < iterations; i++) {
        color.r = sin(color.r * 1.01);
        color.g = cos(color.g * 1.01);
        color.b = sin(color.b * 1.01 + color.r);
    }
    outColor = vec4(abs(color.rgb), 1.0);
}
