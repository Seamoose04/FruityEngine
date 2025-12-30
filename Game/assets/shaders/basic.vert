#version 450 core
layout (location = 0) in vec3 aPos;

out vec4 v_clip_pos;

uniform mat4 u_MVP;

void main() {
    v_clip_pos = u_MVP * vec4(aPos * 0.5, 1.0);
    gl_Position = v_clip_pos;
}