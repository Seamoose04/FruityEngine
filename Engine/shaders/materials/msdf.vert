#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aUV;

uniform mat4 u_MVP;

out vec2 v_UV;

void main() {
	v_UV = aUV;
	gl_Position = u_MVP * vec4(aPos, 1.0);
}
