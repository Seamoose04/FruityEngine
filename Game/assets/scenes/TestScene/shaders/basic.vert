#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_MVP;

void main() {
    gl_Position = vec4(aPos * 0.5, 1.0);
}
