#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "GLSL.h"

class Program;

class Platform {
public:
	Platform(float left, float right, float bottom, float top);

	void init();
	void init_texture();

	void draw(const std::shared_ptr<Program> prog) const;

	float left, right, bottom, top;

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

	std::vector<GLfloat> posBuf;

	std::vector<GLfloat> texBuf;

	glm::mat4 M;
};

#endif