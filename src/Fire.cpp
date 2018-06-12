#include "Fire.h"
#include "stb_image.h"
#include "Program.h"
#include "glm/gtc/matrix_transform.hpp"

Fire::Fire(float x, float y) {
	pos.x = x;
	pos.y = y;

	to = glm::vec2(0.f, 0.f);
	to2 = glm::vec2(0.f, 0.f);
	
	vel = glm::vec2(0.f, 0.f);

	time = 0;
	t = 0;

	upsideDown = false;

	init();
	init_texture();
}

Fire::Fire(float x, float y, bool _upsideDown) {
	pos.x = x;
	pos.y = y;

	to = glm::vec2(0.f, 0.f);
	to2 = glm::vec2(0.f, 0.f);
	
	vel = glm::vec2(0.f, 0.f);

	time = 0;
	t = 0;

	upsideDown = _upsideDown;

	init();
	init_texture();
}

void Fire::init() {
	// Generate VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Generate VBO
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);

	// Copy vertex positions to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(posBuf), posBuf, GL_DYNAMIC_DRAW);

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

void Fire::init_texture() {
	int width, height, channels;
	std::string str = "../resources/fire.png";
	unsigned char* data = stbi_load(str.c_str(), &width, &height, &channels, 4);

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Fire::update(double frametime, std::vector<Water> &waterDroplets) {
	time += frametime;

	if (time > 1.f/(float)framesPerSecond) {
		// Time to update new frame
		time -= 1.f/(float)framesPerSecond;
		to = to2;
		t = time / (float)framesPerSecond;

		if (to2.x >= (7.f / 8.f)) {
			if (to2.y >= (7.f / 8.f)) {
				to2.x = 0.f;
				to2.y = 0.f;
			}
			else {
				to2.x = 0.f;
				to2.y += (1.f / 8.f);
			}
		}
		else {
			to2.x += (1.f / 8.f);
		}
	}
	else {
		// Same frame just change t
		t = (float)time / (float)framesPerSecond;
	}

	checkWaterDropletsCollision(waterDroplets);

	if (upsideDown) {
		M = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y - size, -10.f));
	}
	else {
		M = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y + size, -10.f));
	}
	M = glm::scale(M, glm::vec3(size, size, 1));

	if (upsideDown) {
		M = glm::rotate(M, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	}
}

void Fire::draw(const std::shared_ptr<Program> prog) const {
	prog->setMatrix("M", &M[0][0]);

	prog->setVector2("to", &to[0]);
	prog->setVector2("to2", &to2[0]);

	prog->setFloat("t", t);

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

void Fire::checkWaterDropletsCollision(std::vector<Water> &waterDroplets) {
	for (unsigned int i = 0; i < waterDroplets.size(); i++) {
		Water::Hitbox hit = waterDroplets[i].getHitbox();

		if (hit.right > pos.x - size && hit.left < pos.x + size) {
			// Water droplet is within horizontal coordinates of fire
			if (hit.bottom < pos.y + size && hit.top > pos.y - size) {
				// Water droplet is within vertical coordinates of (lower half of) fire
				waterDroplets.erase(waterDroplets.begin() + i);

				size -= 1;
				continue;
			}
		}
	}
}

void Fire::checkPlayerDamage(Player* player) {
	Player::Hitbox hitbox = player->getHitbox();

	if (hitbox.left < pos.x + size && hitbox.right > pos.x - size) {
		// Player is within horizontal coordinates of fire
		if (!upsideDown && hitbox.bottom < pos.y && hitbox.top > pos.y - size) {
			// Player is within bottom half of fire

			player->health -= 0.01f * size;
		}
		else if (upsideDown && hitbox.bottom < pos.y + size && hitbox.top > pos.y) {
			// Player is within top half of fire

			player->health -= 0.01f * size;
		}
	}
}