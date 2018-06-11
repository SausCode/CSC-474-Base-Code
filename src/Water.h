#pragma once

#ifndef WATER_H
#define WATER_H

#include <memory>
#include "Shape.h"
#include <glm/glm.hpp>
#include "GLSL.h"

class Water {
public:
	Water(glm::vec2 playerPos, glm::vec2 mousePos, std::shared_ptr<Shape> &_shape);

	void update(double frametime);

	void draw(const std::shared_ptr<Program> prog) const;

	glm::vec2 pos, vel;

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;

	int velocity_scalar = 20;
	
	glm::vec3 scale = glm::vec3(20, 20, 1);

	float GRAVITY = 35;

	float angle;

	float color;
};

#endif