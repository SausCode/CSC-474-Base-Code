#define NOMINMAX
#include "Player.h"
#include "Program.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "bone.h"

Player::Player(float xpos, float ypos) {
	pos.x = xpos;
	pos.y = ypos;

	vel = glm::vec2(0.f);

	std::string resourceDirectory = "../resources";

	shape = std::make_shared<Shape>();
    shape->loadMesh(resourceDirectory + "/firefighter.obj");
    shape->resize();
    shape->init();
}

void Player::loadAnimations(const std::string& animationDirectory) {
	//Load player animations
	//readtobone(animationDirectory + "/Walking with CannonChar00.fbx", &root, &all_animation);
	readtobone(animationDirectory + "/walk.fbx", &root, &all_animation);
	readtobone(animationDirectory + "/run.fbx", NULL, &all_animation);
	root->set_animations(&all_animation, animmat, animmatsize);

	//generate the VAO
	glGenVertexArrays(1, &playerVAO);
	glBindVertexArray(playerVAO);

	//generate vertex buffer to hand off to OGL
	glGenBuffers(1, &playerVBO);
	//set the current state to focus on our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);

	vector<vec3> pos;
	vector<unsigned int> imat;

	root->write_to_VBOs(vec3(0, 0, 0), pos, imat);
	size_stick = pos.size();
	//actually memcopy the data - only do this once
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*pos.size(), pos.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//indices of matrix:
	glGenBuffers(1, &imat_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, imat_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*imat.size(), imat.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, (void*)0);

	glBindVertexArray(0);

	//Setup hitboxes
	hitbox.left = shape->min_x * scale.x;
	hitbox.right = shape->max_x * scale.x;
	hitbox.top = shape->max_y * scale.y;
	hitbox.bottom = shape->min_y * scale.y;
}

void Player::updatePlayerAnimation(double frametime) {
	static double totaltime_ms = 0;
	totaltime_ms += frametime * 1000.0;
	static double totaltime_untilframe_ms = 0;
	totaltime_untilframe_ms += frametime * 1000.0;

	for (int ii = 0; ii < 200; ii++)
		animmat[ii] = mat4(1);

	static int current_animation = 1;
	static int next_animation = 1;

	//animation frame system
	int anim_step_width_ms = root->getDuration(next_animation) / root->getKeyFrameCount(next_animation);
	static int frame = 0;
	if (vel.x == 0) {
		/*frame = 0;*/
	}
	else {
		if (totaltime_untilframe_ms >= anim_step_width_ms) {
			totaltime_untilframe_ms = 0;
			frame++;
		}

		if (frame >= root->animation[next_animation]->frames - 1) {
			frame = 0;
			current_animation = next_animation;
		}
	}

	root->play_animation(frame, current_animation, next_animation);
}

void Player::update(double frametime) {
	if (reset) {
		//Resets player back to start
		pos.x = platforms[0].left;
		pos.y = platforms[0].top;
		vel.x = 0;
		vel.y = 0;
	}

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

	static bool lastLeft = false;
	static float rotateAngleNegative = 0.0;
	static float rotateAnglePositive = 0.0;
	//Left
	if (lastLeft) {
		rotateAnglePositive = 0.0;
		//Kinda arbitrary values but I think it looks good
		M = glm::rotate(M, glm::radians(std::min(-rotateY, (float)std::pow(rotateAngleNegative -=-1,1.5))), glm::vec3(0, 1, 0));
	}
	//Right
	else if(!lastLeft){
		rotateAngleNegative = 0.0;
		M = glm::rotate(M, glm::radians(std::max(rotateY, -(float)std::pow(rotateAnglePositive -= -1, 1.5))), glm::vec3(0, 1, 0));
	}

	if (left) {
		lastLeft = true;
	}
	else if (right) {
		lastLeft = false;
	}

	M = glm::scale(M, vec3(.2, .2, .2));
}

void Player::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) {
	glBindVertexArray(playerVAO);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(prog->getUniform("Manim"), 200, GL_FALSE, &animmat[0][0][0]);
	glDrawArrays(GL_LINES, 4, size_stick - 4);

	//prog->setMatrix("M", &M[0][0]);

	glBindVertexArray(0);
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