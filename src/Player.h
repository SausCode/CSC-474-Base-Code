#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "bone.h"
#include "Shape.h"
#include "Platform.h"
#include <glm/glm.hpp>

class Player {
public:
	struct Hitbox {
		float left, right, bottom, top;
	};


	Player(float xpos, float ypos);

	void draw(const std::shared_ptr<Program> prog, bool use_extern_texures);

	void loadAnimations(const std::string& animationDirectory);

	void updatePlayerAnimation(double frametime);

	void update(double frametime);

	bool checkPlatformLeft();
	bool checkPlatformRight();
	bool checkPlatformUp();
	bool checkPlatformDown();

	Hitbox getHitbox();

	glm::vec2 pos, vel;

	bool left = false, right = false, jump = false, reset = false;

	std::vector<Platform> platforms;

	float health = 100.f;

private:
	std::shared_ptr<Shape> shape;

	glm::mat4 M;

	bool isJumping = false;

	float speedEpsilon = .1;

	const int WALK_VEL = 8;
	const int JUMP_VEL = 10;
	const int FRICTION = 45;
	const int GRAVITY = 20;
	float rotateY = -90.0;
	const glm::vec3 scale = glm::vec3(100, 100, 50);

	Hitbox hitbox;

	unsigned int playerVAO = 0;
	unsigned int playerVBO = 0;
	unsigned int imat_VBO = 0;

	bone *root = NULL;
	int size_stick = 0;
	all_animations all_animation;
	mat4 animmat[200];
	int animmatsize = 0;
};

#endif