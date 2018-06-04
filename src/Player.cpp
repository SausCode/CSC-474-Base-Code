#define NOMINMAX
#include "Player.h"
#include "Program.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Player::Player(float xpos, float ypos) {
	pos.x = xpos;
	pos.y = ypos;

	vel = glm::vec2(0.f);

	std::string resourceDirectory = "../resources";

	shape = std::make_shared<Shape>();
    shape->loadMesh(resourceDirectory + "/firefighter.obj");
    shape->resize();
    shape->init();

    hitbox.left = shape->min_x * scale.x;
    hitbox.right = shape->max_x * scale.x;
    hitbox.top = shape->max_y * scale.y;
    hitbox.bottom = shape->min_y * scale.y;

}

void Player::update(double frametime) {
	vel.y -= frametime * GRAVITY;

	// Update velocity based on controls
	if (left) {
		vel.x -= 50 * frametime;
		vel.x = std::max(vel.x, (float)-WALK_VEL);
	}
	if (right) {
		vel.x += 50 * frametime;
		vel.x = std::min(vel.x, (float)WALK_VEL);
	}
	if (!left && !right) {
		vel.x *= frametime * FRICTION;
	}
	if (jump && !isJumping) {
		vel.y = JUMP_VEL;
		isJumping = true;
	}

	// Check velocity based on platform constraints
	if (vel.x < -0.01f) {
		// moving to the left - Check if moving left would hit any platforms
		if (checkPlatformLeft()) {
			// Player would hit a platform moving left!
			vel.x = 0;
		}
	}
	else if (vel.x > 0.01f) {
		// moving to the right - Check if moving right would hit any platforms
		if (checkPlatformRight()) {
			vel.x = 0;
		}
	}

	if (vel.y < 0.01f) {
		// moving down - Check if moving down would hit any platforms
		if (checkPlatformDown()) {
			// Player would hit a platform moving down!
			vel.y = 0;
			isJumping = false;
		}
	}
	else if (vel.y > 0.01f) {
		// moving up - Check if moving up would hit any platforms
		if (checkPlatformUp()) {
			vel.y = 0;
		}
	}



	pos += vel;
	
	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));
	
	M = glm::scale(M, scale);

	M = glm::rotate(M, glm::radians(rotateY), glm::vec3(0, 1, 0));
}

void Player::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) {
	prog->setMatrix("M", &M[0][0]);

	shape->draw(prog, use_extern_texures);
}

Player::Hitbox Player::getHitbox() {
	Player::Hitbox newHitbox;

	newHitbox.left = hitbox.left + pos.x;
	newHitbox.right = hitbox.right + pos.x;
	newHitbox.bottom = hitbox.bottom + pos.y;
	newHitbox.top = hitbox.top + pos.y;

	return newHitbox;
}

bool Player::checkPlatformLeft() {
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

bool Player::checkPlatformRight() {
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

bool Player::checkPlatformUp() {
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

bool Player::checkPlatformDown() {
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