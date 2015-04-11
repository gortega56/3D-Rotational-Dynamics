#version 400

uniform mat4 clip;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 texCoord;

out vec3 worldNormal;
out vec2 uv;

void main()
{
	worldNormal = mat3(clip) * vertexNormal;
	uv = vec2(texCoord);
	gl_Position = clip * vec4(vertexPosition, 1.0);
}