#pragma once

#ifndef WATER_H
#define WATER_H

#include <memory>
#include "Shape.h"
#include <glm/glm.hpp>
#include "GLSL.h"

class Water {
public:
	Water(glm::vec3 _currentPos, glm::vec3 _endingPos, float _xSpeed, std::shared_ptr<Shape> _shaped);

	void update(double frametime);

	void draw(const std::shared_ptr<Program> prog, bool use_extern_texures) const;

	glm::vec3 currentPos, endingPos;

	glm::vec2 pos, vel;

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;
	
	glm::vec3 scale = glm::vec3(20, 20, 50);

	int GRAVITY = 1;
};

#endif