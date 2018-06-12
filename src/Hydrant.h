#pragma once

#ifndef HYDRANT_H
#define HYDRANT_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "GLSL.h"

class Program;
class Player;

class Hydrant {
public:
	Hydrant(glm::vec2 pos);

	void init();
	void init_texture();

	void update(Player* player);

	void draw(const std::shared_ptr<Program> prog) const;

	bool shouldDelete = false;

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

	const float size = 75.f;

	glm::mat4 M;

	glm::vec2 pos;
};

#endif