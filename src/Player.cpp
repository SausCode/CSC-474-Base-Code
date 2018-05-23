#include "Player.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Player::Player(float xpos, float ypos) {
	pos.x = xpos;
	pos.y = ypos;

	vel = glm::vec2(0.f);

	std::string resourceDirectory = "../resources";

	shape = std::make_shared<Shape>();
    shape->loadMesh(resourceDirectory + "/sphere.obj");
    shape->resize();
    shape->init();
}

void Player::update(double frametime) {
	if (left) {
		vel.x -= 50 * frametime;
		vel.x = std::max(vel.x, (float)-WALK_VEL);
	}
	if (right) {
		vel.x += 50 * frametime;
		vel.x = std::min(vel.x, (float)WALK_VEL);
	}
	if (jump && !isJumping) {
		vel.y = JUMP_VEL;
	}



	pos += vel;

	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));
	M = glm::scale(M, scale);
}

void Player::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) {
	prog->setMatrix("M", &M[0][0]);

	shape->draw(prog, use_extern_texures);
}