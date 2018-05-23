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
}

void Player::update(double frametime) {


	if (vel.x < -.01) {
		vel.x += frametime * FRICTION;
	}
	else if (vel.x > .01) {
		vel.x -= frametime * FRICTION;
	}

	//80 is *about* half the height, and so makes him stand on top
	//TODO: make this not a shit way to do collision detection lol
	if (pos.y < platform.top+80) {
		pos.y = platform.top+80;
	}
	else if (pos.y > .01) {
		vel.y -= frametime * GRAVITY;
	}

	if (left) {
		vel.x -= 50 * frametime;
		vel.x = std::max(vel.x, (float)-WALK_VEL);
	}
	if (right) {
		vel.x += 50 * frametime;
		vel.x = std::min(vel.x, (float)WALK_VEL);
	}
	if (jump && pos.y < platform.top + 100) {
		vel.y += frametime*JUMP_VEL;
	}



	pos += vel;
	
	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));
	
	M = glm::scale(M, scale);

	M = glm::rotate(M, glm::radians(rotateY+=1), glm::vec3(0, 1, 0));
}

void Player::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) {
	prog->setMatrix("M", &M[0][0]);

	shape->draw(prog, use_extern_texures);
}