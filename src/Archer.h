#pragma once

#ifndef ARCHER_H
#define ARCHER_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "GLSL.h"

class Program;
class Water;

class Archer {
public:
	Archer(glm::vec2 pos);

	void init();
	void init_texture();

	void update(double frametime);

	void draw(const std::shared_ptr<Program> prog) const;

	void checkWaterDropletsCollision(std::vector<Water> &waterDroplets);

	float health = 100.f;

	bool shouldRemove = false;

	double time_since_last_arrow = 0.0;

	glm::vec2 pos;

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
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	std::vector<GLfloat> posBuf;

	float size = 75.f;

	glm::mat4 M;
};

#endif