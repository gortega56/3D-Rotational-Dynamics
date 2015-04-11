#version 400

uniform sampler2D tex1;

in vec3 worldNormal;
in vec2 uv;

out vec4 fragmentColor;

struct DirectionalLight
{
	vec4 ambientColor;
	vec3 direction;
};

void main()
{
	vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0);
	vec3 direction = vec3(-1.0, -1.0, 1.0);
	DirectionalLight dLight = DirectionalLight(ambientColor, direction);

	vec3 directionToLight = -normalize(dLight.direction);
	float dLightContribution = clamp(dot(normalize(worldNormal), directionToLight), 0.0, 1.0);

	vec4 diffuseColor = texture2D(tex1, uv);

	fragmentColor = (dLightContribution * diffuseColor) + dLight.ambientColor;
	
}