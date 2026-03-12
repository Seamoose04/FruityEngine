#version 450 core

in vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_InputA;
uniform sampler2D u_InputB;
uniform int u_Method;

void main() {
    vec3 colorA = texture(u_InputA, v_UV).rgb;
    vec3 colorB = texture(u_InputB, v_UV).rgb;

    vec3 result;
    switch (u_Method) {
        case 0: result = colorA + colorB; break;
        case 1: result = colorA * colorB; break;
    }

    FragColor = vec4(result, 1.0);
}