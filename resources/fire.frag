#version 330 core

out vec4 color;

in vec2 vertex_tex;

uniform sampler2D tex;

uniform vec2 to;
uniform vec2 to2;

uniform float t;

void main() {
	vec2 new_tex = vec2(vertex_tex.x/8.0, vertex_tex.y/8.0);

	vec4 tcol1 = (1 - t) * texture(tex, new_tex + to);
	vec4 tcol2 = t * texture(tex, new_tex + to2);

	color = tcol1 + tcol2;
}