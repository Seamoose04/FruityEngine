#version 450 core

in vec2 v_UV;

out vec4 FragColor;

uniform sampler2D u_Input;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(u_Input, 0));

    // 3x3 tent filter — softens the upscale
    vec4 result = vec4(0.0);
    result += texture(u_Input, v_UV + vec2(-1, -1) * texelSize) * 0.0625;
    result += texture(u_Input, v_UV + vec2( 0, -1) * texelSize) * 0.125;
    result += texture(u_Input, v_UV + vec2( 1, -1) * texelSize) * 0.0625;
    result += texture(u_Input, v_UV + vec2(-1,  0) * texelSize) * 0.125;
    result += texture(u_Input, v_UV + vec2( 0,  0) * texelSize) * 0.25;
    result += texture(u_Input, v_UV + vec2( 1,  0) * texelSize) * 0.125;
    result += texture(u_Input, v_UV + vec2(-1,  1) * texelSize) * 0.0625;
    result += texture(u_Input, v_UV + vec2( 0,  1) * texelSize) * 0.125;
    result += texture(u_Input, v_UV + vec2( 1,  1) * texelSize) * 0.0625;
    FragColor = result;
}
