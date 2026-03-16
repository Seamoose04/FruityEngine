#version 450 core

uniform vec3 u_Color;

out vec4 frag_color;

void main() {
    frag_color = vec4(u_Color * 1.0, 1.0);
}
