
#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
	vec4 pos = M * (vec4(vertPos,1.0));
	gl_Position = P * V * pos;
}
