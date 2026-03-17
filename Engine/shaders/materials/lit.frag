#version 450 core

in vec3 v_WorldPos;
in vec3 v_Normal;

out vec4 FragColor;

uniform vec3 u_Color;
uniform float u_Roughness;
uniform float u_Metalness;

uniform vec3 u_CameraPos;

struct PointLight {
	vec4 positionAndRadius;
	vec4 colorAndIntensity;
};

layout (std140, binding = 0) uniform LightBlock {
	PointLight u_PointLights[8];
	vec4 u_AmbientColorAndIntensity;
	int u_PointLightCount;
};

void main() {
	vec3 N = normalize(v_Normal);
	vec3 V = normalize(u_CameraPos - v_WorldPos);

	float shininess = mix(256.0, 2.0, u_Roughness);
	
	vec3 specularColor = mix(vec3(1.0), u_Color, u_Metalness);

	vec3 ambient = u_AmbientColorAndIntensity.xyz * u_AmbientColorAndIntensity.w * u_Color;

	vec3 result = ambient;

	for (int i = 0; i < u_PointLightCount; i++) {
		vec3 lightPos = u_PointLights[i].positionAndRadius.xyz;
		float radius = u_PointLights[i].positionAndRadius.w;
		vec3 lightColor = u_PointLights[i].colorAndIntensity.xyz;
		float intensity = u_PointLights[i].colorAndIntensity.w;

		vec3 L = normalize(lightPos - v_WorldPos);
		float dist = length(lightPos - v_WorldPos);

		float attenuation = pow(clamp(1.0 - dist / radius, 0.0, 1.0), 2.0);
		
		float diff = max(dot(N, L), 0.0);
		vec3 diffuse = diff * lightColor * intensity * u_Color;

		vec3 H = normalize(L + V);
		float spec = pow(max(dot(N, H), 0.0), shininess);
		vec3 specular = spec * lightColor * intensity * specularColor;

		result += (diffuse + specular) * attenuation;
	}

	FragColor = vec4(result, 1.0);
}
