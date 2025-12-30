#version 450 core

in vec4 v_clip_pos;

out vec4 FragColor;

void main() {
    vec3  ndc_pos = v_clip_pos.xyz / v_clip_pos.w;
    vec3  dx = dFdx( ndc_pos );
    vec3  dy = dFdy( ndc_pos );

    vec3 N = normalize(cross(dx, dy));
    N *= sign(N.z);
    vec3 L = vec3(1.0, 1.0, 1.0); // direction to the light source
    float NdotL = dot(N, L); 

    vec3 diffuse_color = vec3(0.6, 0.6, 0.6) * NdotL;
    FragColor = vec4(diffuse_color.rgb, 1.0);
}
