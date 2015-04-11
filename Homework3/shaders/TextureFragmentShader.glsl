#version 400

uniform sampler2D tex2;
in vec2 uv;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture2D(tex2, uv);	
}