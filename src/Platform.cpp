#include "Platform.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>

Platform::Platform(float _left, float _right, float _bottom, float _top) {
	left = _left;
	right = _right;
	bottom = _bottom;
	top = _top;

	std::string resourceDirectory = "../resources";

    shape = std::make_shared<Shape>();
    shape->loadMesh(resourceDirectory + "/platform.obj");
    shape->resize();
    shape->init();

    M = glm::translate(glm::mat4(1.f), glm::vec3(left + (right - left)/2.f, top - (top - bottom)/2.f, -60.f));
    M = glm::scale(M, glm::vec3((right - left)/2.f, (top - bottom)/2.f, 50));
}

void Platform::draw(const std::shared_ptr<Program> prog, bool use_extern_texures) const {
	prog->setMatrix("M", &M[0][0]);

	shape->draw(prog, use_extern_texures);
}