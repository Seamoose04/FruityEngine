#version 450 core

in vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_Input;
uniform float u_Threshold;
uniform float u_Knee;

void main() {
    vec3 color = texture(u_Input, v_UV).rgb;

    float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float contribution = smoothstep(u_Threshold, u_Threshold + u_Knee, luminance);
    
    FragColor = vec4(color * contribution, 1.0);
}