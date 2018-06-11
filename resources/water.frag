#version 330 core
out vec4 color;

uniform float c;

void main()
{
	color.rgb = vec3(c,c,1);
}