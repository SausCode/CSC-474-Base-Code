#pragma once

#ifndef WATER_H
#define WATER_H

#include <memory>
#include <vector>
#include "Shape.h"
#include <glm/glm.hpp>
#include "GLSL.h"
#include "Platform.h"


class Water {
public:
	struct Hitbox {
		float left, right, bottom, top;
	};

	Water(glm::vec2 playerPos, glm::vec2 mousePos, glm::vec2 playerVel, std::shared_ptr<Shape> &_shape, std::vector<Platform> &platforms);

	void update(double frametime);

	void draw(const std::shared_ptr<Program> prog) const;

	Hitbox getHitbox();

	bool shouldRemove = false;

	glm::vec2 pos, vel;

private:
	std::shared_ptr<Shape> shape;

	std::vector<Platform> platforms;

	glm::mat4 M;

	int velocity_scalar = 20;
	
	glm::vec3 scale = glm::vec3(20, 20, 1);

	float GRAVITY = 35;

	float angle;

	float color;

	Hitbox hitbox;

	int bounces = 0;

	bool checkPlatformLeft();
	bool checkPlatformRight();
	bool checkPlatformDown();
	bool checkPlatformUp();
};

#endif