#pragma once

#ifndef ARROW_H
#define ARROW_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "GLSL.h"
#include "Platform.h"

class Arrow {
public:
	struct Hitbox {
		float left, right, bottom, top;
	};

	Arrow(glm::vec2 pos, std::vector<Platform> &platforms);

	void init();
	void init_texture();

	void update(double frametime);

	void draw(const std::shared_ptr<Program> prog) const;

	Hitbox getHitbox();

	bool shouldRemove = false;

	glm::vec2 pos, vel;

private:
	unsigned int textureID = 0;
	unsigned int eleBufID = 0;
	unsigned int texBufID = 0;
	unsigned int posBufID = 0;
	unsigned int vaoID = 0;

	GLushort eleBuf[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	GLfloat texBuf[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};

	std::vector<GLfloat> posBuf;

	std::vector<Platform> platforms;

	glm::mat4 M;

	int velocity_scalar = 20;

	float GRAVITY = 35;

	float angle;

	Hitbox hitbox;

	bool checkPlatformLeft();
	bool checkPlatformRight();
	bool checkPlatformDown();
	bool checkPlatformUp();
};

#endif