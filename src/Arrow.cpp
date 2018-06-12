#include "Arrow.h"
#include <cstdlib>
#include "Program.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#if defined(WIN32)
#include <ctgmath>
#else
#include <tgmath.h>
#endif

Arrow::Arrow(glm::vec2 _pos, std::vector<Platform> &_platforms) {
	pos = _pos;

	platforms = _platforms;

	vel.x = (rand() / (float)RAND_MAX) - 1;
	vel.y = (rand() / (float)RAND_MAX);

	vel = glm::normalize(vel);

	vel.x *= velocity_scalar;
	vel.y *= velocity_scalar;

	angle = -(atan2(0, 1) - atan2(vel.y, vel.y));

	posBuf = {
		-25,-75, 0.f, // Bottom left
		25, -75, 0.f, // Bottom right
		25,  75, 0.f, // Top right
		-25, 75, 0.f, // Top left
	};

	hitbox.left = pos.x - 25;
	hitbox.right = pos.x + 25;
	hitbox.top = pos.y + 75;
	hitbox.top = pos.y - 75;

	init();
	init_texture();
}

void Arrow::init() {
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

void Arrow::init_texture() {
	int width, height, channels;
	std::string str = "../resources/arrow.png";
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

void Arrow::update(double frametime) {
	glm::vec2 oldvel = vel;

	vel.y -= (GRAVITY * frametime);
	pos += vel;

	if (pos.y <= 270) {
		shouldRemove = true;
	}

	angle += -(atan2(oldvel.y, oldvel.x) - atan2(vel.y, vel.x));

	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));

	M = glm::rotate(M, angle, glm::vec3(0, 0, 1));
}

void Arrow::draw(const std::shared_ptr<Program> prog) const {
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

Arrow::Hitbox Arrow::getHitbox() {
	Arrow::Hitbox newHitbox;

	newHitbox.left = hitbox.left + pos.x;
	newHitbox.right = hitbox.right + pos.x;
	newHitbox.bottom = hitbox.bottom + pos.y;
	newHitbox.top = hitbox.top + pos.y;

	return newHitbox;
}

bool Arrow::checkPlatformLeft() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.top > (platforms[i].bottom + 0.1f) && hit.bottom < (platforms[i].top - 0.1f)) {
			// Player is within top / bottom of platform
			if (hit.right > platforms[i].right && hit.left < platforms[i].right) {
				// Player is colliding with platform!
				pos.x = platforms[i].right - hitbox.left + 0.1f; // Position of right platform wall MINUS hitbox left of MODEL
				return true;
			}
		}
	}

	return false;
}

bool Arrow::checkPlatformRight() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.top > (platforms[i].bottom + 0.1f) && hit.bottom < (platforms[i].top - 0.1f)) {
			// Player is within top / bottom of platform
			if (hit.left < platforms[i].left && hit.right > platforms[i].left) {
				// Player is colliding with platform!
				pos.x = platforms[i].left - hitbox.right - 0.1f; // Position of left platform wall MINUS hitbox right of MODEL
				return true;
			}
		}
	}

	return false;
}

bool Arrow::checkPlatformDown() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.left < (platforms[i].right - 0.1f) && hit.right > (platforms[i].left + 0.1f)) {
			// Player is within left / right of platform
			if (pos.y - 75 < platforms[i].top) {
				// Player is colliding with platform!
				std::cout << "AAAA" << std::endl;
				return true;
			}
		}
	}

	return false;
}

bool Arrow::checkPlatformUp() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.left < (platforms[i].right - 0.1f) && hit.right > (platforms[i].left + 0.1f)) {
			// Player is within left / right of platform
			if (hit.bottom < platforms[i].bottom && hit.top > platforms[i].bottom) {
				// Player is colliding with platform!
				pos.y = platforms[i].bottom - hitbox.top - 0.1f; // Position of bottom platform wall MINUS hitbox top of MODEL
				return true;
			}
		}
	}

	return false;
}