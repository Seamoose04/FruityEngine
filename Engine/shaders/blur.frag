#version 450 core

in vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_Input;
uniform bool u_Horizontal;
uniform float u_Size;

const float weights[5] = float[](0.227027, 0.194595, 0.121622, 0.054054, 0.016216);

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(u_Input, 0));
    vec2 offset = u_Horizontal ? vec2(texelSize.x * u_Size, 0.0) : vec2(0.0, texelSize.y * u_Size);
    
    vec3 result = texture(u_Input, v_UV).rgb * weights[0];

    for (int i = 1; i < weights.length(); i++) {
        result += texture(u_Input, v_UV + offset * i).rgb * weights[i];
        result += texture(u_Input, v_UV - offset * i).rgb * weights[i];
    }

    FragColor = vec4(result, 1.0);
}
