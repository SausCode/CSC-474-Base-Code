#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "Shape.h"
#include "Platform.h"
#include <glm/glm.hpp>

class Player {
	struct Hitbox {
		float left, right, bottom, top;
	};

public:
	Player(float xpos, float ypos);

	void draw(const std::shared_ptr<Program> prog, bool use_extern_texures);

	void update(double frametime);

	bool checkPlatformLeft();
	bool checkPlatformRight();
	bool checkPlatformUp();
	bool checkPlatformDown();

	Hitbox getHitbox();

	glm::vec2 pos, vel;

	bool left = false, right = false, jump = false;

	std::vector<Platform> platforms;

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;

	bool isJumping = false;

	const int WALK_VEL = 8;
	const int JUMP_VEL = 10;
	const int FRICTION = 45;
	const int GRAVITY = 10;
	float rotateY = 90.0;
	const glm::vec3 scale = glm::vec3(100, 100, 50);

	Hitbox hitbox;
};

#endif