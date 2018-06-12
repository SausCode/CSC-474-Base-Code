#include "Water.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <iostream>
#if defined(WIN32)
#include <ctgmath>
#else
#include <tgmath.h>
#endif


Water::Water(glm::vec2 playerPos, glm::vec2 mousePos, glm::vec2 playerVel, std::shared_ptr<Shape> &_shape, std::vector<Platform> &_platforms) {
	pos = playerPos;
	pos.y += 50;

	shape = _shape;

	platforms = _platforms;

	vel = glm::normalize(mousePos - playerPos);

	angle = (3.1415f / 2.f) + -(atan2(0, 1) - atan2(vel.y, vel.x));

	float rand_x = ((rand() / (float)RAND_MAX) / 5.f) + 0.8f;
	float rand_y = ((rand() / (float)RAND_MAX) / 5.f) + 0.8f;

	vel.x *= (velocity_scalar * rand_x);
	vel.y *= (velocity_scalar * rand_y);

	vel.x += playerVel.x;
	vel.y += playerVel.y;

	color = (rand() / (float)RAND_MAX);

	hitbox.left = shape->min_x * scale.x;
    hitbox.right = shape->max_x * scale.x;
    hitbox.top = shape->max_y * scale.y;
    hitbox.bottom = shape->min_y * scale.y;
}

void Water::update(double frametime) {
	glm::vec2 oldvel = vel;

	vel.y -= (GRAVITY * frametime);
	pos += vel;

	if (vel.x <= -0.01f) {
		if (checkPlatformLeft()) {
			vel.x *= -0.5f;
			vel.y *= 0.5f;
			bounces++;
		}
	}
	else if (vel.y >= 0.01f) {
		if (checkPlatformRight()) {
			vel.x *= -0.5f;
			vel.y *= 0.5f;
			bounces++;
		}
	}

	if (vel.y <= -0.01f) {
		if (checkPlatformDown()) {
			vel.y *= -0.5f;
			vel.x *= 0.5f;
			bounces++;
		}
	}
	else if (vel.y >= 0.01f) {
		if (checkPlatformUp()) {
			vel.y *= -0.5f;
			vel.x *= 0.5f;
			bounces++;
		}
	}

	if (bounces > 3) {
		shouldRemove = true;
	}

	angle += -(atan2(oldvel.y, oldvel.x) - atan2(vel.y, vel.x));

	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));

	M = glm::rotate(M, angle, glm::vec3(0, 0, 1));

	M = glm::scale(M, scale);
}

void Water::draw(const std::shared_ptr<Program> prog) const {
	prog->setMatrix("M", &M[0][0]);
	prog->setFloat("c", color);

	shape->draw(prog, false);
}

Water::Hitbox Water::getHitbox() {
	Water::Hitbox newHitbox;

	newHitbox.left = hitbox.left + pos.x;
	newHitbox.right = hitbox.right + pos.x;
	newHitbox.bottom = hitbox.bottom + pos.y;
	newHitbox.top = hitbox.top + pos.y;

	return newHitbox;
}

bool Water::checkPlatformLeft() {
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

bool Water::checkPlatformRight() {
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

bool Water::checkPlatformDown() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.left < (platforms[i].right - 0.1f) && hit.right > (platforms[i].left + 0.1f)) {
			// Player is within left / right of platform
			if (hit.top > platforms[i].top && hit.bottom < platforms[i].top) {
				// Player is colliding with platform!
				pos.y = platforms[i].top - hitbox.bottom + 0.1f; // Position of top platform wall MINUS hitbox bottom of MODEL
				return true;
			}
		}
	}

	return false;
}

bool Water::checkPlatformUp() {
	Hitbox hit = getHitbox();

	for (unsigned int i = 0; i < platforms.size(); i++) {
		if (hit.left < (platforms[i].right - 0.1f) && hit.right > (platforms[i].left + 0.1f)) {
			// Player is within left / right of platform
			if (hit.bottom < platforms[i].bottom && hit.top > platforms[i].bottom) {
				std::cout << hit.top << "      " << platforms[i].bottom << std::endl;
				// Player is colliding with platform!
				pos.y = platforms[i].bottom - hitbox.top - 0.1f; // Position of bottom platform wall MINUS hitbox top of MODEL
				return true;
			}
		}
	}

	return false;
}