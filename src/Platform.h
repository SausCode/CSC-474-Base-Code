#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

#include <memory>
#include "Shape.h"
#include <glm/glm.hpp>

class Platform {
public:
	Platform(float left, float right, float bottom, float top);

	void draw(const std::shared_ptr<Program> prog, bool use_extern_texures) const;

	float left, right, bottom, top;

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;
};

#endif