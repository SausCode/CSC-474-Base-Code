#include "Hydrant.h"
#include "stb_image.h"
#include "Program.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

Hydrant::Hydrant(glm::vec2 _pos) {
	pos = _pos;

	posBuf = {
		pos.x - size, pos.y - size, -10.f, 	// Bottom left
		pos.x + size, pos.y - size, -10.f, 	// Bottom right
		pos.x + size, pos.y + size, -10.f,  // Top right
		pos.x - size, pos.y + size, -10.f,	// Top left
	};

	init();
	init_texture();
}

void Hydrant::init() {
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

	M = glm::mat4(1.f);
}

void Hydrant::init_texture() {
	int width, height, channels;
	std::string str = "../resources/hydrant.png";
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

void Hydrant::update(Player* player) {
	// Check if player is within size of hydrant - refresh water
	Player::Hitbox hit = player->getHitbox();

	if (hit.left < pos.x + size && hit.right > pos.x - size) {
		// Within horizontal coordinates of hydrant
		if (hit.top > pos.y - size && hit.bottom < pos.y + size) {
			// Within vertical coordinates of hydrant
			player->water = 100.f;
		}
	}
}

void Hydrant::draw(const std::shared_ptr<Program> prog) const {
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