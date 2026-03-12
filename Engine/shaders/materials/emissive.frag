#version 450 core

uniform vec3 u_Color;
uniform float u_Intensity;

out vec4 frag_color;

void main() {
    frag_color = vec4(u_Color * u_Intensity, 1.0);
}
