#include "Archer.h"
#include "stb_image.h"
#include "Program.h"
#include "Water.h"
#include <glm/gtc/matrix_transform.hpp>

Archer::Archer(glm::vec2 _pos) {
	pos = _pos;

	posBuf = {
		pos.x - size, pos.y - size, -10.f, // Bottom left
		pos.x + size, pos.y - size, -10.f, // Bottom right
		pos.x + size, pos.y + size, -10.f, // Top right
		pos.x - size, pos.y + size, -10.f, // Top left
	};

	init();
	init_texture();
}

void Archer::init() {
	// Generate VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Generate VBO
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);

	// Copy vertex positions to buffer
	glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(GLfloat), posBuf.data(), GL_DYNAMIC_DRAW);

	// Generate VBO for textures
	glGenBuffers(1, &texBufID);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);

	// Copy vertex coordinates to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(texBuf), texBuf, GL_STATIC_DRAW);

	// Generate Element array
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);

	// Copy element indices to buffers
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eleBuf), eleBuf, GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

void Archer::init_texture() {
	int width, height, channels;
	std::string str = "../resources/archer.png";
	unsigned char* data = stbi_load(str.c_str(), &width, &height, &channels, 4);

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Archer::update(double frametime) {
	if (health <= 0) {
		shouldRemove = true;
	}

	time_since_last_arrow += frametime;
}

void Archer::draw(const std::shared_ptr<Program> prog) const {
	prog->setMatrix("M", &M[0][0]);
	glBindVertexArray(vaoID);

	int pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	int tex = prog->getAttribute("vertTex");
	GLSL::enableVertexAttribArray(tex);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawElements(GL_TRIANGLES, sizeof(eleBuf), GL_UNSIGNED_SHORT, (void*)0);

	GLSL::disableVertexAttribArray(tex);
	GLSL::disableVertexAttribArray(pos);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Archer::checkWaterDropletsCollision(std::vector<Water> &waterDroplets) {
	for (unsigned int i = 0; i < waterDroplets.size(); i++) {
		Water::Hitbox hit = waterDroplets[i].getHitbox();

		if (hit.right > pos.x - size && hit.left < pos.x + size) {
			// Water droplet is within horizontal coordinates of fire
			if (hit.bottom < pos.y + size && hit.top > pos.y - size) {
				// Water droplet is within vertical coordinates of (lower half of) fire
				waterDroplets.erase(waterDroplets.begin() + i);

				health -= 1;
				continue;
			}
		}
	}
}