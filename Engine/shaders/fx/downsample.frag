#version 450 core

in vec2 v_UV;

out vec4 FragColor;

uniform sampler2D u_Input;

void main() {
    FragColor = texture(u_Input, v_UV);
}
