#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in float right_or_left;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float health;
uniform float width;

void main() {
	vec3 pos;

	if (right_or_left > 0.5) {
		// It's on the right
		float x = (width * health) / 100.0;
		pos = vec3(vertPos.x + x, vertPos.y, vertPos.z);
	}
	if (right_or_left < 0.5) {
		// It's on the left
		pos = vertPos;
	}

	gl_Position = P * V * (M * vec4(pos, 1.0));
}