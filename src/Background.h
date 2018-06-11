#pragma once

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "GLSL.h"

class Program;

class Background {
public:
	Background(int screenWidth, int screenHeight);

	void init();
	void init_texture();

	void update(glm::vec2 cameraPos);

	void draw(const std::shared_ptr<Program> prog) const;

private:
	int screenWidth = 0;
	int screenHeight = 0;

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
		0.0, 1.4,
		8.0, 1.4,
		8.0, 0.2,
		0.0, 0.2,
	};

	std::vector<GLfloat> posBuf;

	glm::mat4 M;

	glm::vec2 pos;
};

#endif