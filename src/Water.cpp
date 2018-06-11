#include "Water.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <iostream>
#if defined(WIN32)
#include <ctgmath>
#else
#include <tgmath.h>
#endif


Water::Water(glm::vec2 playerPos, glm::vec2 mousePos, std::shared_ptr<Shape> &_shape) {
	pos = playerPos;
	pos.y += 50;

	shape = _shape;

	vel = glm::normalize(mousePos - playerPos);

	angle = (3.1415f / 2.f) + -(atan2(0, 1) - atan2(vel.y, vel.x));

	float rand_x = ((rand() / (float)RAND_MAX) / 5.f) + 0.8f;
	float rand_y = ((rand() / (float)RAND_MAX) / 5.f) + 0.8f;

	vel.x *= (velocity_scalar * rand_x);
	vel.y *= (velocity_scalar * rand_y);

	color = (rand() / (float)RAND_MAX);
}

void Water::update(double frametime) {
	glm::vec2 oldvel = vel;

	vel.y -= (GRAVITY * frametime);
	pos += vel;

	angle += -(atan2(oldvel.y, oldvel.x) - atan2(vel.y, vel.x));

	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));

	M = glm::rotate(M, angle, glm::vec3(0, 0, 1));

	M = glm::scale(M, scale);
}

void Water::draw(const std::shared_ptr<Program> prog) const {
	prog->setMatrix("M", &M[0][0]);
	prog->setFloat("c", color);

	shape->draw(prog, false);
}