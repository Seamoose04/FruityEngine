#version 450 core

in vec2 v_UV;

uniform sampler2D u_Atlas;
uniform vec4 u_Color;

out vec4 frag_color;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main() {
	vec3 s = texture(u_Atlas, v_UV).rgb;
	float dist = median(s.r, s.g, s.b);
	float w = fwidth(dist) * 0.5;
	float alpha = smoothstep(0.5 - w, 0.5 + w, dist);
	frag_color = vec4(u_Color.rgb, u_Color.a * alpha);
}
