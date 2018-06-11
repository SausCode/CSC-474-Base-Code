#include "Water.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>

Water::Water(glm::vec3 _currentPos, glm::vec3 _endingPos, float _xSpeed, std::shared_ptr<Shape> _shape) {

	currentPos = _currentPos;
	endingPos = _endingPos;

	shape = _shape;

	pos.x = currentPos.x;
	pos.y = currentPos.y;

	vel.x = _xSpeed;

	M = glm::translate(glm::mat4(1.f), endingPos);
	M = glm::scale(M, glm::vec3(10,10,5));
}

void Water::update(double frametime) {

	vel.y -= GRAVITY;

	pos += vel;

	M = glm::translate(glm::mat4(1), glm::vec3(pos, -10.f));

	M = glm::rotate(M, glm::radians(90.f), glm::vec3(0, 0, 1));

	M = glm::scale(M, scale);

}

void Water::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) const {
	
	prog->setMatrix("M", &M[0][0]);

	shape->draw(prog, use_extern_texures);
}