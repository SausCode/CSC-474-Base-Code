#include "Waterbar.h"
#include "Program.h"

#include <algorithm>

Waterbar::Waterbar(float _width) {
	width = _width;

	color = glm::vec3(0.f, 0.f, 1.f);
	init();
}

void Waterbar::init() {
	// Generate VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Generate VBO
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);

	// Copy vertex positions to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(posBuf), posBuf, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &waterBufID);
	glBindBuffer(GL_ARRAY_BUFFER, waterBufID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(waterBuf), waterBuf, GL_STATIC_DRAW);

	// Generate Element array
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eleBuf), eleBuf, GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

void Waterbar::update(float _water) {
	water = std::max(_water, 0.f);
}

void Waterbar::draw(const std::shared_ptr<Program> prog) const {
	prog->setFloat("health", water);
	prog->setFloat("width", width);

	prog->setVector3("c", color.x, color.y, color.z);

	glBindVertexArray(vaoID);

	int pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	int right_left = prog->getAttribute("right_or_left");
	GLSL::enableVertexAttribArray(right_left);
	glBindBuffer(GL_ARRAY_BUFFER, waterBufID);
	glVertexAttribPointer(right_left, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);

	glDrawElements(GL_TRIANGLES, sizeof(eleBuf), GL_UNSIGNED_SHORT, (void*)0);

	//GLSL::disableVertexAttribArray(right_left);
	GLSL::disableVertexAttribArray(pos);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}