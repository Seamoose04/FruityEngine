#version 450 core

in vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_Input;
uniform float u_Exposure = 1.0;

void main() {
    vec3 hdr = texture(u_Input, v_UV).rgb;

    // Reinhard tone mapping — brings HDR values back into 0-1 range
    vec3 mapped = vec3(1.0) - exp(-hdr * u_Exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}
