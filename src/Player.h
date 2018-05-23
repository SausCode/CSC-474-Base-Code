#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "Shape.h"
#include "Platform.h"
#include <glm/glm.hpp>

class Player {
public:
	Player(float xpos, float ypos);

	void draw(const std::shared_ptr<Program> prog, bool use_extern_texures);

	void update(double frametime);

	glm::vec2 pos, vel;

	bool left = false, right = false, jump = false;

	Platform platform = Platform(0, 0, 0, 0);

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;

	bool isJumping;

	const int WALK_VEL = 50;
	const int JUMP_VEL = 100;
	const int FRICTION = 35;
	const int GRAVITY = 5;
	float rotateY = 0.0;
	const glm::vec3 scale = glm::vec3(100, 100, 50);
};

#endif