#version 400

uniform mat4 clip;

in vec3 vertexPosition;
in vec2 texCoord;

out vec2 uv;

void main()
{
	uv = vec2(texCoord);
	gl_Position = clip * vec4(vertexPosition, 1.0);
}